// Main dev: Mateusz Miroslaw Lis

#include "ShotEvaluator.hpp"


ShotEvaluator::ShotEvaluator(HaarDetector& frontal, HaarDetector& profile, HaarDetector& eye,
                             ShotClassifier& faceClassifier, int eyeThreshold)
    : frontal_face_detector(frontal), profile_face_detector(profile), eye_detector(eye),
      face_classifier(faceClassifier), eye_classifier(0, eyeThreshold) {}

ClassificationResult ShotEvaluator::evaluate(const cv::Mat& image, std::vector<cv::Rect>& allFaces, std::vector<cv::Rect>& eyes) {
    // Assume image is already resized externally
    std::vector<cv::Rect> frontal_faces = frontal_face_detector.detect(image);
    eyes = eye_detector.detect(image);

    std::vector<cv::Rect> profile_left = profile_face_detector.detect(image);

    // Detect right profiles
    cv::Mat flipped;
    cv::flip(image, flipped, 1);
    std::vector<cv::Rect> profile_right_flipped = profile_face_detector.detect(flipped);
    std::vector<cv::Rect> profile_right;
    for (const auto& rect : profile_right_flipped) {
        int x = image.cols - (rect.x + rect.width);
        profile_right.emplace_back(cv::Rect(x, rect.y, rect.width, rect.height));
    }

    std::vector<cv::Rect> all_profiles = profile_left;
    all_profiles.insert(all_profiles.end(), profile_right.begin(), profile_right.end());

    allFaces = frontal_faces;
    allFaces.insert(allFaces.end(), all_profiles.begin(), all_profiles.end());

    int classification = face_classifier.classify(allFaces);
    if (classification == 1 || classification == 2) {
        if (eye_classifier.classify(eyes) == 2) {
            classification = 2;
        }
    }

    // Construct the ClassificationResult using one-hot encoding
    ClassificationResult result;
    switch (classification) {
        case 0:
            result.predictedType = ShotType::WIDE;
            result.probabilities = {
                {ShotType::WIDE, 1.0},
                {ShotType::MEDIUM, 0.0},
                {ShotType::CLOSE_UP, 0.0}
            };
            break;
        case 1:
            result.predictedType = ShotType::MEDIUM;
            result.probabilities = {
                {ShotType::WIDE, 0.0},
                {ShotType::MEDIUM, 1.0},
                {ShotType::CLOSE_UP, 0.0}
            };
            break;
        case 2:
            result.predictedType = ShotType::CLOSE_UP;
            result.probabilities = {
                {ShotType::WIDE, 0.0},
                {ShotType::MEDIUM, 0.0},
                {ShotType::CLOSE_UP, 1.0}
            };
            break;
        default:
            result.predictedType = ShotType::UNKNOWN;
            result.probabilities = {
                {ShotType::WIDE, 0.0},
                {ShotType::MEDIUM, 0.0},
                {ShotType::CLOSE_UP, 0.0},
                {ShotType::UNKNOWN, 1.0}
            };
            break;
    }

    return result;
}
