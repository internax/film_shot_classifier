#include <opencv2/opencv.hpp>
#include "HaarDetector.hpp"
#include "ShotClassifier.hpp"

// Assuming ShotType and ClassificationResult are defined in a separate header
#include "UserStructs.hpp"

class ShotEvaluator {
public:
    ShotEvaluator(HaarDetector& frontal, HaarDetector& profile, HaarDetector& eye, ShotClassifier& faceClassifier, int eyeThreshold);

    classification_result evaluate(const cv::Mat& image, std::vector<cv::Rect>& allFaces, std::vector<cv::Rect>& eyes);

private:
    HaarDetector& frontal_face_detector;
    HaarDetector& profile_face_detector;
    HaarDetector& eye_detector;
    ShotClassifier& face_classifier;
    ShotClassifier eye_classifier;
};
