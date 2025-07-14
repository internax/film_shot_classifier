#include <opencv2/opencv.hpp>
#include <iostream>
#include "../include/FileLoader.hpp"
#include "../include/HaarDetector.hpp"
#include "../include/ShotClassifier.hpp"
#include "../include/ShotEvaluator.hpp"
#include "../include/UserStructs.hpp"
#include "TestDatasetEval.hpp"



// Detection thresholds
int WIDE_THRESHOLD = 3000;
int CLOSEUP_THRESHOLD = 50000;
int EYE_THRESHOLD = 2000;



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

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <image_folder_path>" << std::endl;
        return 1;
    }

    std::string input_folder = argv[1];
    cv::Mat frame;

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
    
    TestDatasetEval wide_shot(ShotType::WIDE);

    while (image_loader.hasNextFrame())
    {
        frame.release();
        frame = image_loader.nextFrame();

        // Preprocess the frame
        
        processor.LoadFrame(frame);
        processor.resizeImage(1280, 720);
        processor.equalizeHistogram();
        frame = processor.GetProcessedImage();

        std::vector<cv::Rect> faces, eyes;
        ClassificationResult classification_result = evaluator.evaluate(frame, faces, eyes);

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
        
        wide_shot.check(classification_result.predictedType);

        // Show processed frame with annotations
        cv::imshow("Processed Image", frame);
        cv::waitKey(200);
        cv::destroyWindow("Processed Image");
    }
    
    std::cout << "accuracy: " << wide_shot.GetEvalResult() << std::endl;

    return 0;
}
