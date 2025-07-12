#include "../include/ShotClassifier.hpp"

ShotClassifier::ShotClassifier(int smallest_thresh, int closeup_thresh)
    : smallest_face_threshold(smallest_thresh), closeup_face_threshold(closeup_thresh)
{}

int ShotClassifier::classify(const std::vector<cv::Rect>& shot_features)
{
    if (shot_features.empty()) {
        return 0; // no faces detected
    }

    const cv::Rect& smallest_face = shot_features.back();
    const cv::Rect& largest_face = shot_features.front();

    if (largest_face.area() < smallest_face_threshold) {
        return 0; // largest face too small
    }

    // Check for closeup: face size is large enough and not too many faces
    if (largest_face.area() >= closeup_face_threshold && shot_features.size() < 3) {
        return 2;  // closeup
    }

    return 1; // medium shot
}
