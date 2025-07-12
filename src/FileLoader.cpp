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


namespace fs = std::filesystem;

void ImageLoader::loadImagePathsFromDirectory(const std::string& directory) {
    image_paths.clear();
    current_frame_index = 0;

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            std::string ext = entry.path().extension().string();

            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || ext == ".tiff") {
                image_paths.push_back(path);
            }
        }
    }

    std::sort(image_paths.begin(), image_paths.end());
}

bool ImageLoader::hasNextFrame() const
{
    return current_frame_index < image_paths.size();
}

cv::Mat& ImageLoader::nextFrame() {
    if (current_frame_index >= image_paths.size()) {
        throw std::runtime_error("No more frames available");
    }

    image.release();

    image = cv::imread(image_paths[current_frame_index]);
    if (image.empty()) {
        throw std::runtime_error("Failed to load image: " + image_paths[current_frame_index]);
    }

    current_frame_index++;
    return image;
}

double ImageLoader::getCurrentTimestamp() const
{
    return 0;
}


void VideoLoader::openVideoFromPath(const std::string & path)
{
    if (video.isOpened())
        video.release();
    
    video.open(path);
       if (!video.isOpened()) {
           throw std::runtime_error("Could not open video file: " + path);
       }

       current_frame_index = 0;
}


bool VideoLoader::hasNextFrame() const
{
    return video.isOpened() && current_frame_index < static_cast<size_t>(video.get(cv::CAP_PROP_FRAME_COUNT));
}

cv::Mat & VideoLoader::nextFrame() {
    
    if (!video.isOpened()) {
        throw std::runtime_error("Video file is not opened.");
    }

    if (!hasNextFrame()) {
        throw std::runtime_error("No more frames available");
    }
    current_frame.release();
    video >> current_frame;
    
    if (current_frame.empty()) {
        throw std::runtime_error("Failed to read next frame.");
    }

    current_frame_index++;
    return current_frame;
}

double VideoLoader::getCurrentTimestamp() const {
    return video.get(cv::CAP_PROP_POS_MSEC);
}



// Preprocessing Implementation
Preprocessing::~Preprocessing() = default;

void Preprocessing::LoadFrame(cv::Mat& new_image) {
    cv::resize(new_image, image, cv::Size(1280, 720));
    //cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    //cv::equalizeHist(image, image);
    // cv::GaussianBlur(image, image, cv::Size(5, 5), 1.5);
   // cv::bilateralFilter(image, image, 9, 75, 75);
}

cv::Mat& Preprocessing::GetProcessedImage() {
    return image;
}

