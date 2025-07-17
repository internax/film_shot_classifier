//
//  sceneChangeDetector.hpp
//  Film_type_classifier
//
//  Developed by Marek Tatýrek
//

#ifndef sceneChangeDetector_hpp
#define sceneChangeDetector_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

bool isSceneChanged(const cv::Mat& prev, const cv::Mat& curr, double threshold = 0.5, int pixelDiffThreshold = 60);

#endif /* sceneChangeDetector_hpp */
