//
//  sceneChangeDetector.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 14.07.2025.
//

#include "SceneChangeDetect.hpp"

bool isSceneChanged(const cv::Mat& prev, const cv::Mat& curr, double threshold, int pixelDiffThreshold)
{
    if (prev.empty() || curr.empty() || prev.size() != curr.size())
        return false;
    

    if (prev.channels() != 1)
        throw std::runtime_error("Number of chanels of image is not 1");

    if (curr.channels() != 1)
        throw std::runtime_error("Number of chanels of image is not 1");
    
    cv::Mat diff, thresh;
    cv::absdiff(prev, curr, diff);
    cv::threshold(diff, thresh, pixelDiffThreshold, 255, cv::THRESH_BINARY);

    double changedPixels = cv::countNonZero(thresh);
    double totalPixels = curr.rows * curr.cols;
    double changeRatio = changedPixels / totalPixels;

    return changeRatio >= threshold;
}
