//
//  UserStructs.hpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 06.06.2025.
//

#ifndef UserStructs_hpp
#define UserStructs_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

/**
 * @struct DetectedFeature
 * @brief Represents a single detected feature in an image, with label and bounding box.
 *
 * This structure is used to return information about each detected object,
 * such as its class label and location in the image.
 */
struct DetectedFeature {
    std::string label;    ///< Type or name of the detected object (e.g., "face")
    cv::Rect boundingBox; ///< Bounding box of the detected object in image coordinates
};

/**
 * @struct ShotFeatures
 * @brief Represents extracted geometric and area-based properties from a video frame.
 *
 * ShotFeatures holds intermediate data used for shot type classification. It includes
 * statistics like the number of detected objects, the total and largest object area,
 * and the coordinates of their centers.
 *
 * The `object_centers` vector is ordered by object size, with the largest first.
 */
struct ShotFeatures {
    int object_count = 0;                ///< Number of detected objects in the frame

    double largest_object_area = 0.;     ///< Area of the largest object detected
    double total_object_area = 0.;       ///< Sum of all object areas
    double total_area = 0.;              ///< Total area of the frame (width * height)

    // for good shot classification + we can add some statistical metrics
    std::vector<cv::Point2f> object_centers; ///< Center points of detected objects, ordered by size
};

/**
 * @enum ShotType
 * @brief Enumeration representing the type of cinematic shot.
 *
 * Used to categorize frames into typical shot sizes.
 */
enum class ShotType {
    CLOSE_UP,   ///< A close-up shot, typically a large face
    MEDIUM,     ///< A medium shot, e.g., upper body
    WIDE,       ///< A wide shot, showing full figures or environment
    UNKNOWN     ///< Could not determine shot type
};

/**
 * @struct ClassificationResult
 * @brief Contains the result of shot type classification.
 *
 * Holds both the most likely predicted shot type and a probability
 * distribution over all possible shot types.
 *
 *Set probability of detected ShotType to 1 if there is not better metrics
 */
struct classification_result {
    ShotType predictedType = ShotType::UNKNOWN;             ///< Most probable shot type, redundant but efective
    std::map<ShotType, double> probabilities;               ///< Probability distribution across shot types
};

/**
 * @struct FilmStatisticsEvalConfig
 * @brief Configuration structure for controlling statistical evaluation of film analysis.
 *
 * This structure allows customization of how the statistical evaluator processes and outputs
 * data derived from frame-by-frame shot classification. It includes control parameters for
 * sampling, entropy analysis, cut detection, and export settings.
 */
struct FilmStatisticsEvalConfig {
    size_t input_step = 1;                          ///< Frame step size – how many frames to skip during analysis (e.g., 1 = every frame, 2 = every second frame)
    size_t input_oversample = 1;                    ///< Number of frames to buffer for oversampling before producing aggregate result

    size_t entropy_window_size = 20;                ///< Window size for calculating mean entropy over time
    size_t entropy_variance_window_size = 30;       ///< Window size for entropy variance calculation (0 = disabled)

    double cut_detect_entropy_threshold = 1;        ///< Threshold value for absolute entropy to trigger cut detection

    bool output_ratios_series = false;              ///< Whether to output ratio data (e.g., object area / frame area)
    size_t output_shot_type_time_series;
    size_t output_prob_time_series = 1;             ///< Interval for outputting classification probability time series (0 = disabled)
    size_t output_entropy_time_series = 1;          ///< Interval for exporting entropy over time (0 = disabled)
    size_t output_entropy_variance_time_series = 1; ///< Interval for exporting entropy variance over time (0 = disabled)
    size_t output_cut_detection_time_series = 1;    ///< Interval for exporting detected cuts (0 = disabled)

    double cut_detection_entropy_treshold = 1.3;    ///< Absolute entropy value required to consider cut (used as hard trigger)
    size_t cut_detection_history_window_size = 15;  ///< Number of previous frames to consider for cut decision history
    double cut_detection_entropy_diff_treshold = 0.3; ///< Required difference in entropy between frames to confirm a cut
};
#endif /* UserStructs_hpp */
