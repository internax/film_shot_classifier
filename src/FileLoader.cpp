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
#include <opencv2/videoio.hpp>
#include <iostream>

VideoLoader::VideoLoader(const std::string& path) : InputSource(path), cap(path) {
    if (!cap.isOpened()) {
        throw std::runtime_error("Failed to open video file: " + path);
    }
}

VideoLoader::~VideoLoader() {
    if (cap.isOpened()) cap.release();
}

bool VideoLoader::hasNextFrame() const {
    return cap.isOpened() && cap.get(cv::CAP_PROP_POS_FRAMES) < cap.get(cv::CAP_PROP_FRAME_COUNT);
}

cv::Mat& VideoLoader::nextFrame() {
    if (!hasNextFrame()) {
        throw std::runtime_error("No more frames available");
    }
    cap >> currentFrame;
    if (currentFrame.empty()) {
        throw std::runtime_error("Failed to read next frame from video");
    }
    timestamp = cap.get(cv::CAP_PROP_POS_MSEC);
    return currentFrame;
}

double VideoLoader::getCurrentTimestamp() const {
    return timestamp;
}

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

bool is_image_file(const std::string& path) {
    static const std::vector<std::string> exts = {".jpg", ".jpeg", ".png", ".bmp", ".tiff", ".tif"};
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return std::find(exts.begin(), exts.end(), ext) != exts.end();
}

bool is_video_file(const std::string& path) {
    static const std::vector<std::string> exts = {".mp4", ".avi", ".mov", ".mkv", ".wmv", ".flv", ".webm"};
    std::string ext = std::filesystem::path(path).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return std::find(exts.begin(), exts.end(), ext) != exts.end();
}

cv::Mat load_image(const std::string& path) {
    ImageLoader loader(path);
    if (loader.hasNextFrame()) {
        cv::Mat frame = loader.nextFrame();
        if (frame.empty()) {
            throw std::runtime_error("Error: Frame is empty");
        }
        Preprocessing processor;
        processor.LoadFrame(frame);
        return processor.GetProcessedImage();
    } else {
        throw std::runtime_error("No frame available");
    }
}

std::vector<cv::Mat> load_video_frames(const std::string& path) {
    VideoLoader loader(path);
    std::vector<cv::Mat> frames;
    while (loader.hasNextFrame()) {
        cv::Mat frame = loader.nextFrame();
        if (frame.empty()) {
            break;
        }
        Preprocessing processor;
        processor.LoadFrame(frame);
        frames.push_back(processor.GetProcessedImage());
    }
    return frames;
}

void display_image(const std::string& path) {
    ImageLoader loader(path);
    if (loader.hasNextFrame()) {
        std::cout << "Image frame is available" << std::endl;
        cv::Mat frame = loader.nextFrame();
        if (frame.empty()) {
            std::cout << "Error: Frame is empty" << std::endl;
            return;
        }
        Preprocessing processor;
        processor.LoadFrame(frame);
        cv::Mat processed = processor.GetProcessedImage();
        cv::imshow("Loaded Image", processed);
        cv::waitKey(0);
    } else {
        std::cout << "No frame available" << std::endl;
    }
}

void display_video(const std::string& path) {
    VideoLoader loader(path);
    int frame_count = 0;
    while (loader.hasNextFrame()) {
        cv::Mat& frame = loader.nextFrame();
        if (frame.empty()) {
            break;
        }
        double timestamp = loader.getCurrentTimestamp();
        std::cout << "Frame: " << frame_count << ", Timestamp: " << timestamp << " ms" << std::endl;
        Preprocessing processor;
        processor.LoadFrame(frame);
        cv::Mat processedFrame = processor.GetProcessedImage();
        cv::imshow("Video Frame", processedFrame);
        if (cv::waitKey(30) == 27) break; // Press 'ESC' to exit
        frame_count++;
    }
    std::cout << "Total frames read: " << frame_count << std::endl;
}

void display_media(const std::string& path) {
    if (!std::filesystem::exists(path)) {
        std::cerr << "File does not exist: " << path << std::endl;
        return;
    }
    if (is_image_file(path)) {
        display_image(path);
    } else if (is_video_file(path)) {
        display_video(path);
    } else {
        std::cerr << "Unsupported file type: " << path << std::endl;
    }
}

