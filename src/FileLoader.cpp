//
//  FileLoader.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 20.05.2025.
//

#include "../include/FileLoader.hpp"
#include <opencv2/opencv.hpp>
#include <stdexcept>
#include <filesystem>
#include <algorithm>

// ImageLoader Implementation
bool ImageLoader::hasNextFrame() const {
    return !frameReturned;
}

cv::Mat& ImageLoader::nextFrame() {
    if (!loaded) {
        image = cv::imread(source_path);
        if (image.empty()) {
            throw std::runtime_error("Failed to load image: " + source_path);
        }
        loaded = true;
    }
    if (!hasNextFrame()) {
        throw std::runtime_error("No more frames available");
    }
    frameReturned = true;
    return image;
}

double ImageLoader::getCurrentTimestamp() const {
    return 0.0; // Single images don't have timestamps
}

std::vector<std::string> ImageLoader::getOrderedImagePaths(const std::string& directory) {
    std::vector<std::string> paths;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            paths.push_back(entry.path().string());
        }
    }
    std::sort(paths.begin(), paths.end());
    return paths;
}

// Preprocessing Implementation
Preprocessing::~Preprocessing() = default;

void Preprocessing::LoadFrame(cv::Mat& new_image) {
    cv::resize(new_image, image, cv::Size(1280, 720));
    // cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    // cv::equalizeHist(image, image);
    // cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
   // cv::bilateralFilter(image, image, 9, 75, 75);
}

cv::Mat& Preprocessing::GetProcessedImage() {
    return image;
}

