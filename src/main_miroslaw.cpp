//#include <opencv2/opencv.hpp>
//#include <iostream>
//#include "HaarDetector.hpp"
//#include "ShotClassifier.hpp"
//#include "ShotEvaluator.hpp"
//
//// Detection parameters (curr best = 80% on all classes)
//int WIDE_THRESHOLD = 3000;
//int CLOSEUP_THRESHOLD = 50000;
//int EYE_THRESHOLD = 2000; 
//
////Example use of HaarDetector, ShotClassifier and ShotEvaluator
//int main() {
//
//    // Haar detectors paths
//    std::string frontal_path = "haarcascade_frontalface_alt2.xml";
//    std::string profile_path = "haarcascade_profileface.xml";
//    std::string eye_path = "haarcascade_eye.xml";
//
//    // Create needed objects
//    HaarDetector frontal_detector(frontal_path);
//    HaarDetector profile_detector(profile_path);
//    HaarDetector eye_detector(eye_path);
//
//    ShotClassifier face_classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
//    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, face_classifier, EYE_THRESHOLD);
//    
//    
//    // Load test image
//    std::string test_image_path = "wide/1.jpg"; 
//    cv::Mat image = cv::imread(test_image_path);
//
//    if (image.empty()) {
//        std::cerr << "Failed to load image: " << test_image_path << std::endl;
//        return -1;
//    }
//
//    // Resize test image (should be done with Preprocess)
//    cv::resize(image, image, cv::Size(1280, 720));
//
//    // Perform evaluation 
//    std::vector<cv::Rect> faces, eyes;
//    int result = evaluator.evaluate(image, faces, eyes);
//
//    std::cout << "Shot classification result: " << result << std::endl;
//    std::cout << "Faces detected: " << faces.size() << ", Eyes detected: " << eyes.size() << std::endl;
//
//    return 0;
//}
