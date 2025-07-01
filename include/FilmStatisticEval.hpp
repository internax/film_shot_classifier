//
//  FilmStatisticEval.hpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#ifndef FilmStatisticEval_hpp
#define FilmStatisticEval_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <numeric>
#include <fstream>
#include "UserStructs.hpp"

/**
 * @class FilmStatistics
 * @brief Collects and analyzes shot type data across a sequence of video frames.
 *
 * The FilmStatistics class is responsible for aggregating classification results
 * from multiple frames and producing summary statistics about shot types used
 * in a video. It also supports time-based analysis and exporting results.
 *
 * This class is typically used at the end of a processing pipeline, after each
 * frame has been classified. It stores both a timeline and cumulative counts of
 * different shot types.
 *
 * The analysis can be configured to skip frames using a configurable `step`,
 * which allows subsampling of the video.
 *
 * Example usage:
 * @code
 *   FilmStatisticsEvalConfig cfg;
 *
 *   FilmStatistics stats;
 *   stats.addConfigurationStruct(cfg);
 *   stats.addFrameResult(timestamp, result);
 *   stats.printSummary();
 *   stats.exportToCSV("shots.csv");
 * @endcode
 *
 * @author Marek Tatýrek
 * @date 2025
 * @see ClassificationResult
 * @see ShotType
 */
class FilmStatistics
{
    /**
     * @brief Count of each shot type encountered in the video.
     */
    std::map<ShotType, int> shot_counts;
    
    /**
     * @brief Timeline of aggregated shot type probabilities at given timestamps.
     * Each entry maps a timestamp (double) to a map of ShotType and their probabilities.
     */
    std::vector<std::pair<double, std::map<ShotType, double>>> prob_timeline;
    
    /**
     * @brief Timeline of detected shot types at given timestamps.
     * Each entry maps a timestamp (double) to the detected ShotType.
     */
    std::vector<std::pair<double, ShotType>> shot_type_timeline;
    
    /**
     * @brief Timeline of entropy values computed over a sliding window of samples.
     * Each entry maps a timestamp (double) to the entropy value.
     */
    std::vector<std::pair<double, double>> enthropy_timeline;
    
    /**
     * @brief Timeline of entropy variance values computed over a sliding window.
     * Each entry maps a timestamp (double) to the entropy variance value.
     */
    std::vector<std::pair<double, double>> enthropy_variance_timeline;
    
    /**
     * @brief Timeline of cut detection flags at given timestamps.
     * Each entry maps a timestamp (double) to a boolean indicating if a cut was detected.
     */
    std::vector<std::pair<double, bool>> cut_detection_timeline;
    
    /**
     * @brief Sliding window storing classification results for oversampling.
     */
    std::deque<classification_result> oversampling_sliding_window;
    
    /**
     * @brief Sliding window storing classification results for entropy computation.
     */
    std::deque<classification_result> entropy_sliding_window;
    
    /**
     * @brief Sliding window storing entropy variance values.
     */
    std::deque<double> entropy_variance_sliding_window;
    
    /**
     * @brief Total number of frames processed so far.
     */
    int total_frames = 0;
    
    /**
     * @brief Number of frames that have been evaluated and included in statistics.
     */
    int evaluated_frames = 0;
    
    /**
     * @brief Current oversampled classification result being processed.
     */
    classification_result current_sample_oversampled;
    
    /**
     * @brief Current entropy value computed from the sliding window.
     */
    double entropy = 0.0;
    
    /**
     * @brief Timestamp in milliseconds of the current frame/sample.
     */
    double timestamp_ms = 0.0;
    
    /**
     * @brief Current entropy variance value computed from the sliding window.
     */
    double entropy_variance = 0.0;
    
    /**
     * @brief Configuration parameters for the FilmStatistics evaluation.
     */
    FilmStatisticsEvalConfig config;
    
    /**
     * @brief Number of initial frames to delay before starting analysis.
     */
    int start_delay = 0;
    
    /**
     * @brief Duration of a single frame in milliseconds used for time measurement.
     */
    double frame_time_measurement;
    
    /**
     * @brief Normalizes the aggregated probabilities so they sum to 1.
     * @param aggregated_probs Map of ShotType to their aggregated probabilities to normalize.
     */
    void normalizeProbs(std::map<ShotType, double> & aggregated_probs);
    
    /**
     * @brief Aggregates probabilities from a sliding window of classification results.
     * @param sliding_window Deque containing classification results to aggregate.
     * @param aggregated_probs Output map of ShotType to aggregated probability values.
     */
    void aggregateSlidingWindowProbs(std::deque<classification_result> & sliding_window, std::map<ShotType, double> & aggregated_probs);
    
    /**
     * @brief Finds the shot type with the maximum probability from aggregated probabilities and updates the sample.
     * @param aggregated_probs Map of ShotType to their aggregated probabilities.
     * @param sample Classification result to update with the shot type having max probability.
     */
    void findShotTypeMaxProb(std::map<ShotType, double> & aggregated_probs, classification_result & sample);
    
    /**
     * @brief Performs oversampling on input data to improve statistical robustness.
     */
    void oversampleInputData();
    
    /**
     * @brief Computes the entropy of the current sliding window of classification results.
     */
    void computeEntropy();
    
    /**
     * @brief Computes the variance of the entropy values over the sliding window.
     */
    void computeEntropyVariance();
    
    /**
     * @brief Outputs collected statistics to the console or log.
     */
    void outputStatistics();
    
    /**
     * @brief Appends the current sample's aggregated probabilities and timestamp to a timeline.
     * @param timeline Vector to append the timestamp and aggregated shot type probabilities.
     */
    void appendSampleToTimeline(std::vector<std::pair<double, std::map<ShotType, double>>> & timeline);
    
    /**
     * @brief Appends the current sample's shot type and timestamp to a timeline.
     * @param timeline Vector to append the timestamp and shot type.
     */
    void appendSampleToTimeline(std::vector<std::pair<double, ShotType>> & timeline);
    
    /**
     * @brief Appends the current entropy value and timestamp to a timeline.
     * @param timeline Vector to append the timestamp and entropy value.
     */
    void appendEntropyToTimeline(std::vector<std::pair<double, double>> & timeline);
    
    /**
     * @brief Appends the current entropy variance value and timestamp to a timeline.
     * @param timeline Vector to append the timestamp and entropy variance value.
     */
    void appendEntropyVarianceToTimeline(std::vector<std::pair<double, double>> & timeline);
    
    /**
     * @brief Appends the current cut detection flag and timestamp to a timeline.
     * @param timeline Vector to append the timestamp and cut detection boolean.
     */
    void appendCutDetectionToTimeline(std::vector<std::pair<double, bool>> & timeline);
    
public:
    /**
     * @brief Default constructor (analyzes every frame).
     */
    FilmStatistics() = default;

    /**
     * @brief Adds configuration parameters for the evaluation.
     * @param cfg Configuration struct containing evaluation parameters.
     */
    void addConfigurationStruct(FilmStatisticsEvalConfig const & cfg);

    /**
     * @brief Adds the classification result for a single frame.
     *
     * @param timestampMs Timestamp of the frame in milliseconds.
     * @param result Classification result containing shot type.
     */
    void addFrameResult(const double timestampMs, const classification_result & result);

    /**
     * @brief Exports all collected statistics to a CSV file.
     *
     * The CSV includes both timeline data and summary shot counts.
     *
     * @param path File path to export the data to.
     */
    void exportToCSV(const std::string& path) const;

    /**
     * @brief Prints a textual summary of the shot distribution to the console.
     */
    void printSummary() const;
    
    
    // -- temporary --
    /**
     * @brief Gets the entropy timeline.
     * @return Reference to vector of timestamp and entropy pairs.
     */
    std::vector<std::pair<double, double>> & getEntropy(){return enthropy_timeline; };

    /**
     * @brief Gets the entropy variance timeline.
     * @return Reference to vector of timestamp and entropy variance pairs.
     */
    std::vector<std::pair<double, double>> & getEntropyVAriance(){return enthropy_variance_timeline; };

    /**
     * @brief Gets the shot type timeline.
     * @return Reference to vector of timestamp and shot type pairs.
     */
    std::vector<std::pair<double, ShotType>> & getShotType(){return shot_type_timeline;}
    // -- temporary --
};

#endif /* FilmStatisticEval_hpp */
