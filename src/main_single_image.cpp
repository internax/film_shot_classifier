#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "FilmShotClassifier.hpp"
#include "FileLoader.hpp"
#include "HaarDetector.hpp"
#include "ShotClassifier.hpp"
#include "ShotEvaluator.hpp"

// Detection parameters (curr best = 80% on all classes)
int WIDE_THRESHOLD = 3000;
int CLOSEUP_THRESHOLD = 50000;
int EYE_THRESHOLD = 2000; 

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }

    try {
        // Load image using your ImageLoader
        ImageLoader loader(argv[1]);

        if (!loader.hasNextFrame()) {
            std::cout << "No frame available" << std::endl;
            return -1;
        }

        // Get the frame
        cv::Mat frame = loader.nextFrame();

        if (frame.empty()) {
            std::cerr << "Error: Frame is empty" << std::endl;
            return -1;
        }

        // Preprocess frame
        Preprocessing processor;
        processor.LoadFrame(frame);
        cv::Mat processed = processor.GetProcessedImage();

        if (processed.empty()) {
            std::cerr << "Error: Processed frame is empty" << std::endl;
            return -1;
        }

        std::cout << "Successfully resized frame to: " << processed.size().width 
                  << "x" << processed.size().height << std::endl;

        // --- Classification part ---

        // Haar detectors paths - relative paths are used, detectors should stay in "haar" folder
        std::string frontal_path = "../haar/haarcascade_frontalface_alt2.xml";
        std::string profile_path = "../haar/haarcascade_profileface.xml";
        std::string eye_path = "../haar/haarcascade_eye.xml";

        HaarDetector frontal_detector(frontal_path);
        HaarDetector profile_detector(profile_path);
        HaarDetector eye_detector(eye_path);

        ShotClassifier face_classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
        ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, face_classifier, EYE_THRESHOLD);

        // Perform evaluation on processed image
        std::vector<cv::Rect> faces, eyes;
        int classification_result = evaluator.evaluate(processed, faces, eyes);
        
        std::cout << "Shot classification result: " << classification_result << std::endl;
        std::cout << "Faces detected: " << faces.size() << ", Eyes detected: " << eyes.size() << std::endl;
        
        // Draw rectangles around faces and eyes
        for (const auto& face : faces) {
            cv::rectangle(processed, face, cv::Scalar(0, 255, 0), 2);  // Green
        }
        
        for (const auto& eye : eyes) {
            cv::rectangle(processed, eye, cv::Scalar(255, 0, 0), 2);   // Blue
        }
        
        // Display the processed image with bounding boxes
        cv::imshow("Processed Image", processed);
        cv::waitKey(0);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
