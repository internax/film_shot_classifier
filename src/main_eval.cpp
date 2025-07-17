// Main dev: Mateusz Miroslaw Lis

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include <string>
#include <numeric>
#include "FileLoader.hpp"
#include "HaarDetector.hpp"
#include "ShotClassifier.hpp"
#include "ShotEvaluator.hpp"
#include "UserStructs.hpp"


//for use enhanced double clasifier set DOUBLE_CLASSIFIER to 1 else 0
#define DOUBLE_CLASSIFIER 0

// Class labels
std::vector<std::string> classes = { "closeup", "medium", "wide" };



std::string shotTypeToString(ShotType type) {
    switch (type) {
        case ShotType::CLOSE_UP: return "closeup";
        case ShotType::MEDIUM: return "medium";
        case ShotType::WIDE: return "wide";
        default: return "unknown";
    }
}

ShotType stringToShotType(const std::string& str) {
    if (str == "closeup") return ShotType::CLOSE_UP;
    if (str == "medium") return ShotType::MEDIUM;
    if (str == "wide") return ShotType::WIDE;
    return ShotType::UNKNOWN;
}




struct Metrics {
    int TP = 0, FP = 0, FN = 0, TN = 0;
};



void computeAndPrintMetrics(const std::string& label, const Metrics& m) {
    double precision = 0.0;
    double recall = 0.0;
    double f1 = 0.0;
    double accuracy = 0.0;

    if ((m.TP + m.FP) > 0) precision = (double)m.TP / (m.TP + m.FP);
    if ((m.TP + m.FN) > 0) recall = (double)m.TP / (m.TP + m.FN);
    if ((precision + recall) > 0) f1 = 2 * precision * recall / (precision + recall);

    accuracy = (double)(m.TP + m.TN) / (m.TP + m.FP + m.FN + m.TN);

    std::cout << "Metrics for " << label << ":\n";
    std::cout << "Precision: " << precision << "\n";
    std::cout << "Recall: " << recall << "\n";
    std::cout << "F1 Score: " << f1 << "\n";
    std::cout << "Accuracy: " << accuracy << "\n\n";
}


void printConfusionMatrix(const int matrix[3][3], const std::vector<std::string>& labels) {
    std::cout << "\nConfusion Matrix:\n";
    std::cout << "Actual \\ Predicted";

    for (const auto& label : labels) {
        std::cout << "\t" << label;
    }
    std::cout << "\n";

    for (int i = 0; i < 3; ++i) {
        std::cout << labels[i];
        for (int j = 0; j < 3; ++j) {
            std::cout << "\t\t" << matrix[i][j];
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

#if DOUBLE_CLASSIFIER

// Parameters
int constexpr WIDE_THRESHOLD = 10000;
int constexpr CLOSEUP_THRESHOLD = 50000;
int constexpr EYE_THRESHOLD = 2000;

constexpr int RESOLUTION_WIDTH = 1280;
constexpr int RESOLUTION_HEIGHT = 720;

int main() {

    // Test dataset, as contained in archive
    std::string base_path = "../test";

    // Haar detectors, as contained in archive
    std::string frontal_path = "../haar/haarcascade_frontalface_alt2.xml";
    std::string profile_path = "../haar/haarcascade_profileface.xml";
    std::string eye_path = "../haar/haarcascade_eye.xml";

    // Create objects for detection and classification
    HaarDetector frontal_detector(frontal_path);
    HaarDetector profile_detector(profile_path);
    HaarDetector eye_detector(eye_path);
    ShotClassifier classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, classifier, EYE_THRESHOLD);

    ClassificationResult classification_result_final;
    ClassificationResult classification_result;
    ClassificationResult classification_result_eq;
    
    cv::Mat frame_eq;
    cv::Mat frame;
    
    std::vector<cv::Rect> faces, eyes;
    
    Preprocessing processor;
    
    // Confusion matrix
    std::map<std::string, Metrics> metrics_map;
    int confusion_matrix[3][3] = {0};
    int total_images = 0;
    int correct_predictions = 0;

    // Iterate over images in test class folder
    for (const std::string& cls : classes) {
    
        std::cout << "Running on: " << cls << "\n";
        
        std::string folder_path = base_path + "/" + cls;
        ImageLoader image_loader(folder_path);
        

        while (image_loader.hasNextFrame()) {
            frame = image_loader.nextFrame();
            if (frame.empty()) {
                std::cerr << "Error: Empty frame encountered in folder: " << cls << std::endl;
                continue;
            }

            processor.LoadFrame(frame);
            processor.resizeImage(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
            frame = processor.GetProcessedImage();
            processor.equalizeHistogram();
            frame_eq = processor.GetProcessedImage();
    
            classification_result = evaluator.evaluate(frame, faces, eyes);
            classification_result_eq = evaluator.evaluate(frame_eq, faces, eyes);
    
            if((classification_result.predictedType == ShotType::WIDE )||(classification_result_eq.predictedType == ShotType::WIDE))
                classification_result_final.predictedType = ShotType::WIDE;
    
            if((classification_result.predictedType == ShotType::MEDIUM )||(classification_result_eq.predictedType == ShotType::MEDIUM))
                classification_result_final.predictedType = ShotType::MEDIUM;
    
            if((classification_result.predictedType == ShotType::CLOSE_UP )||(classification_result_eq.predictedType == ShotType::CLOSE_UP))
                classification_result_final.predictedType = ShotType::CLOSE_UP;

            ShotType predicted = classification_result_final.predictedType;
            ShotType actual = stringToShotType(cls);

            // Update confusion matrix
            int actual_index = static_cast<int>(actual);
            int predicted_index = static_cast<int>(predicted);
            if (actual_index < 3 && predicted_index < 3) {
                confusion_matrix[actual_index][predicted_index]++;
            }

            // One-vs-all metrics
            for (const std::string& target_cls : classes) {
                ShotType target_type = stringToShotType(target_cls);
                Metrics& m = metrics_map[target_cls];

                if (actual == target_type && predicted == target_type) m.TP++;
                else if (actual != target_type && predicted == target_type) m.FP++;
                else if (actual == target_type && predicted != target_type) m.FN++;
                else m.TN++;
            }

            if (predicted == actual) correct_predictions++;
            total_images++;
        }
    }

    for (const auto& pair : metrics_map) {
        computeAndPrintMetrics(pair.first, pair.second);
    }

    // Final metrics print
    std::cout << "Overall accuracy: " << static_cast<double>(correct_predictions) / total_images << std::endl;

    printConfusionMatrix(confusion_matrix, classes);

    return 0;
}


#else // sigle classifier

// Parameters
int constexpr WIDE_THRESHOLD = 3000;
int constexpr CLOSEUP_THRESHOLD = 50000;
int constexpr EYE_THRESHOLD = 2000;

constexpr int RESOLUTION_WIDTH = 1280;
constexpr int RESOLUTION_HEIGHT = 720;


int main() {

    // Test dataset, as contained in archive
    std::string base_path = "../test";

    // Haar detectors, as contained in archive
    std::string frontal_path = "../haar/haarcascade_frontalface_alt2.xml";
    std::string profile_path = "../haar/haarcascade_profileface.xml";
    std::string eye_path = "../haar/haarcascade_eye.xml";

    // Create objects for detection and classification
    HaarDetector frontal_detector(frontal_path);
    HaarDetector profile_detector(profile_path);
    HaarDetector eye_detector(eye_path);
    ShotClassifier classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, classifier, EYE_THRESHOLD);
    ClassificationResult result;
    
    cv::Mat frame;
    Preprocessing processor;

    // Confusion matrix
    std::map<std::string, Metrics> metrics_map;
    int confusion_matrix[3][3] = {0};
    int total_images = 0;
    int correct_predictions = 0;

    // Iterate over images in test class folder
    for (const std::string& cls : classes) {

        std::cout << "Running on: " << cls << "\n";

        std::string folder_path = base_path + "/" + cls;
        ImageLoader image_loader(folder_path);

        while (image_loader.hasNextFrame()) {
            frame = image_loader.nextFrame();
            if (frame.empty()) {
                std::cerr << "Error: Empty frame encountered in folder: " << cls << std::endl;
                continue;
            }

            
            processor.LoadFrame(frame);
            processor.resizeImage(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
            frame = processor.GetProcessedImage();

            std::vector<cv::Rect> faces, eyes;
            result = evaluator.evaluate(frame, faces, eyes);

            ShotType predicted = result.predictedType;
            ShotType actual = stringToShotType(cls);

            // Update confusion matrix
            int actual_index = static_cast<int>(actual);
            int predicted_index = static_cast<int>(predicted);
            if (actual_index < 3 && predicted_index < 3) {
                confusion_matrix[actual_index][predicted_index]++;
            }

            // One-vs-all metrics
            for (const std::string& target_cls : classes) {
                ShotType target_type = stringToShotType(target_cls);
                Metrics& m = metrics_map[target_cls];

                if (actual == target_type && predicted == target_type) m.TP++;
                else if (actual != target_type && predicted == target_type) m.FP++;
                else if (actual == target_type && predicted != target_type) m.FN++;
                else m.TN++;
            }

            if (predicted == actual) correct_predictions++;
            total_images++;
        }
    }

    for (const auto& pair : metrics_map) {
        computeAndPrintMetrics(pair.first, pair.second);
    }

    // Final metrics print
    std::cout << "Overall accuracy: " << static_cast<double>(correct_predictions) / total_images << std::endl;

    printConfusionMatrix(confusion_matrix, classes);

    return 0;
}
#endif





