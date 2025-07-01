//
//  ResultDisplayer.hpp
//  Film_type_classifier
//
//  Created by Peter... on 21.05.2025.
//

#ifndef ResultDisplayer_hpp
#define ResultDisplayer_hpp

#include <stdio.h>
#include "FilmStatisticEval.hpp"

/**
 * @class ResultDisplayer
 * @brief Provides graphical or textual output for film analysis results.
 *
 * The ResultDisplayer is responsible for visualizing the data collected
 * in the FilmStatistics object. This includes summaries, distributions of shot types,
 * timelines, or other outputs meant for the user.
 *
 * This class currently stores a copy of the FilmStatistics object.
 * For performance or design flexibility, a reference-based approach
 * may also be considered depending on use case.
 *
 * Future implementations may include rendering to GUI, charts, or image overlays.
 *
 * @see FilmStatistics
 */
class ResultDisplayer
{
    FilmStatistics film_stats; ///< Collected statistics about the film (copied on construction)

public:
    /**
     * @brief Constructs the displayer with a copy of the film statistics.
     * @param stats The FilmStatistics object containing the processed analysis data.
     */
    ResultDisplayer(const FilmStatistics& stats) : film_stats(stats) {}

    /**
     * @brief Default destructor.
     */
    ~ResultDisplayer() = default;

    cv::Mat GetPlot(/*input_dataset, xlabel, ylabel*/);
};

#endif /* ResultDisplayer_hpp */
