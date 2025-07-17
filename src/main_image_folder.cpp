// Main dev: Mateusz Miroslaw Lis

#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/FileLoader.hpp"
#include "../include/HaarDetector.hpp"
#include "../include/ShotClassifier.hpp"
#include "../include/ShotEvaluator.hpp"
#include "../include/UserStructs.hpp"

//for use enhanced double clasifier set DOUBLE_CLASSIFIER to 1
#define DOUBLE_CLASSIFIER 0

// Detection thresholds
constexpr int WIDE_THRESHOLD = 10000;
constexpr int CLOSEUP_THRESHOLD = 50000;
constexpr int EYE_THRESHOLD = 2000;

constexpr int RESOLUTION_WIDTH = 1280;
constexpr int RESOLUTION_HEIGHT = 720;

std::string shotTypeToString(ShotType type) {
    switch (type) {
        case ShotType::CLOSE_UP: return "CLOSE_UP";
        case ShotType::MEDIUM: return "MEDIUM";
        case ShotType::WIDE: return "WIDE";
        case ShotType::UNKNOWN:
        default: return "UNKNOWN";
    }
}


int main(int argc, char** argv)
{


    std::string input_folder = argv[1];
    cv::Mat frame;
    cv::Mat frame_eq;

    // Load all images from the directory
    ImageLoader image_loader("test/wide");

    // Haar detector paths
    std::string frontal_path = "haar/haarcascade_frontalface_alt2.xml";
    std::string profile_path = "haar/haarcascade_profileface.xml";
    std::string eye_path = "haar/haarcascade_eye.xml";

    // Initialize detectors and classifier
    HaarDetector frontal_detector(frontal_path);
    HaarDetector profile_detector(profile_path);
    HaarDetector eye_detector(eye_path);
    ShotClassifier face_classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, face_classifier, EYE_THRESHOLD);
    
    Preprocessing processor;
    ClassificationResult classification_result_final;
    ClassificationResult classification_result;
    ClassificationResult classification_result_eq;
    std::vector<cv::Rect> faces, eyes;

#if DOUBLE_CLASSIFIER
    while (image_loader.hasNextFrame())
    {
        frame.release();
        frame = image_loader.nextFrame();

        // Preprocess the frame
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
    
        
        std::cout << "Shot classification: " << shotTypeToString(classification_result_final.predictedType)
                  << " | Faces: " << faces.size()
                  << " | Eyes: " << eyes.size() << std::endl;

        // Draw bounding boxes
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }

        for (const auto& eye : eyes) {
            cv::rectangle(frame, eye, cv::Scalar(255, 0, 0), 2);
        }

        // Show processed frame with annotations
        cv::imshow("Processed Image", frame);
        cv::waitKey(0);
        cv::destroyWindow("Processed Image");
    }


#else
    while (image_loader.hasNextFrame())
    {
        frame.release();
        frame = image_loader.nextFrame();
        
        // Preprocess the frame
        processor.LoadFrame(frame);
        processor.resizeImage(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
        frame = processor.GetProcessedImage();
        
        classification_result = evaluator.evaluate(frame, faces, eyes);
        
        std::cout << "Shot classification: " << shotTypeToString(classification_result.predictedType)
        << " | Faces: " << faces.size()
        << " | Eyes: " << eyes.size() << std::endl;
        
        // Draw bounding boxes
        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(0, 255, 0), 2);
        }
        
        for (const auto& eye : eyes) {
            cv::rectangle(frame, eye, cv::Scalar(255, 0, 0), 2);
        }
        
        // Show processed frame with annotations
        cv::imshow("Processed Image", frame);
        cv::waitKey(0);
        cv::destroyWindow("Processed Image");
    }
#endif

    return 0;
}
