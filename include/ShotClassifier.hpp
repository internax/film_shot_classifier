#ifndef ShotClassifier_hpp
#define ShotClassifier_hpp

#include <vector>
#include <opencv2/opencv.hpp>

class ShotClassifier
{
    int smallest_face_threshold; // minimal area for smallest face
    int medium_face_threshold;   // minimal area for medium shot
    int closeup_face_threshold;  // minimal area for closeup

public:
    ShotClassifier(int smallest_thresh = 5000, int closeup_thresh = 30000);

    int classify(const std::vector<cv::Rect>& shot_features);
};

#endif /* ShotClassifier_hpp */
