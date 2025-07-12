//
//  FeatureProccesor.hpp
//  Film_type_classifier
//
//  Created by Miroslaw on 21.05.2025.
//

#ifndef FeatureProccesor_hpp
#define FeatureProccesor_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "UserStructs.hpp"

/**
 * @class ShotFeatureExtractor
 * @brief Extracts numerical and spatial features from detected objects in a video frame.
 *
 * ShotFeatureExtractor analyzes a frame and its detected objects and computes a set of
 * high-level features that describe the visual composition. These features are then used
 * for classification of shot type or composition analysis.
 *
 * This class is stateless and suitable for reuse across multiple frames.
 *
 * Example:
 * @code
 *   ShotFeatureExtractor extractor;
 *   ShotFeatures features = extractor.extract(frame, detectedObjects);
 * @endcode
 *
 * @see ShotFeatures
 * @see DetectedFeature
 */
class ShotFeatureExtractor {
    double returnTotalArea(const cv::Mat& frame); ///< Computes total frame area
    double returnLaregstObjectArea(const std::vector<DetectedFeature>& features); ///< Finds the largest detected object
    double returnTotalObjectArea(const std::vector<DetectedFeature>& features); ///< Sums up all detected object areas

public:
    ShotFeatureExtractor() = default;
    ~ShotFeatureExtractor() = default;

    /**
     * @brief Extracts shot-level features from a frame and its detected objects.
     *
     * @param frame Input image frame from a video.
     * @param features Detected objects in the frame (e.g., faces, bodies).
     * @return A populated ShotFeatures structure.
     */
    ShotFeatures extract(const cv::Mat& frame, const std::vector<DetectedFeature>& features);
};


/**
 * @class ShotClassifier
 * @brief Classifies the type of cinematic shot based on extracted visual features.
 *
 * ShotClassifier takes a ShotFeatures input and produces a classification result.
 * Typically used after feature extraction from a single video frame.
 *
 * The classification logic is based on heuristics, such as the relative size of
 * detected objects compared to the frame.
 *
 * @see ShotFeatures
 * @see ClassificationResult
 */
class ShotClassifier {
public:
    ShotClassifier() = default;
    ~ShotClassifier() = default;

    /**
     * @brief Classifies the shot type using extracted features.
     *
     * @param features The extracted shot-level features.
     * @return A ClassificationResult with the predicted type and probabilities.
     */
    ClassificationResult classify(const ShotFeatures& features) const;
};

#endif /* FeatureProccesor_hpp */
