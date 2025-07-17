//
//  TestDatasetEval.hpp
//  Film_type_classifier
//
//  Developed by Marek Tatýrek
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

    ShotType desired_type;
    
    int true_detect;
    int counter;
    
public:

    TestDatasetEval(ShotType type) : desired_type(type)
    {
        true_detect = 0;
        counter = 0;
    }

    /**
     * @brief Adds a predicted image classification result to the evaluation set.
     * @param Image The image to be evaluated (prediction logic assumed to be applied externally).
     */
    bool check(ShotType input_type)
    {
        counter ++;
        if(input_type == desired_type)
        {
            true_detect ++;
            return true;
        }
        return false;
    }

    /**
     * @brief Computes and returns the evaluation result (e.g., accuracy).
     * @return A floating-point score representing classifier performance.
     */
    double GetEvalResult()
    {
        return static_cast<double>(true_detect)/static_cast<double>(counter);
    }
};
