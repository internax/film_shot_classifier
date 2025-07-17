//
//  FeatureDetector.cpp
//  Film_type_classifier
//
//  Main developer: Mateusz Miroslaw Lis
//

#include "HaarDetector.hpp"
#include <algorithm>

HaarDetector::HaarDetector(std::string& cascade_path) {
    if (!cascade.load(cascade_path)) {
        throw std::runtime_error("Failed to load Haar cascade from: " + cascade_path);
    }
}

std::vector<cv::Rect> HaarDetector::detect(const cv::Mat& image) {
    std::vector<cv::Rect> faces;
    cv::Mat gray;

    // Convert to grayscale if needed
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    // Detect faces
    cascade.detectMultiScale(gray, faces, 1.1, 3, 0, cv::Size(30, 30));

    // Sort bounding boxes, biggest is first
    std::sort(faces.begin(), faces.end(), [](const cv::Rect& a, const cv::Rect& b) {
        return a.area() > b.area();
    });

    return faces;
}
