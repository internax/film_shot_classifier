//
//  TestDatasetEval.hpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 06.06.2025.
//

#ifndef TestDatasetEval_hpp
#define TestDatasetEval_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "UserStructs.hpp"

#endif /* TestDatasetEval_hpp */


/**
 * @class TestDatasetEval
 * @brief Evaluates classification accuracy against a labeled test dataset.
 *
 * The `TestDatasetEval` class is designed to compare predicted classification results
 * with a predefined set of ground truth labels. It supports loading the ground truth data,
 * adding prediction samples, and computing an evaluation score (e.g., accuracy).
 *
 * This is useful for testing and validating the performance of the shot classifier
 * on labeled datasets.
 *
 * Example usage:
 * @code
 *   TestDatasetEval evaluator;
 *   evaluator.loadGroundTruth("ground_truth.csv");
 *   evaluator.addImageSample(predictedLabel);
 *   double accuracy = evaluator.GetEvalResult();
 * @endcode
 *
 * @author Marek Tatýrek
 * @date 2025
 * @see ClassificationResult
 */
class TestDatasetEval
{
    std::vector<classification_result> GroundTruth; ///< Ground truth classification results for comparison

public:

    /**
     * @brief Loads the ground truth classification labels from a file.
     * @param load_path Path to the CSV or data file containing ground truth labels.
     */
    void loadGroundTruth(std::string load_path);

    /**
     * @brief Adds a predicted image classification result to the evaluation set.
     * @param Image The image to be evaluated (prediction logic assumed to be applied externally).
     */
    void addImageSample(cv::Mat Image);

    /**
     * @brief Computes and returns the evaluation result (e.g., accuracy).
     * @return A floating-point score representing classifier performance.
     */
    double GetEvalResult();
};
