//
//  FilmStatisticEval.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#include "FilmStatisticEval.hpp"
//future improvements: compensate different framerates based on the time measurement

// input data computing functions
void FilmStatistics::normalizeProbs(std::map<ShotType, double> & aggregated_probs)
{
    if(!aggregated_probs.empty())
    {
        double total = 0.0;
        for (const auto& [type, value] : aggregated_probs)
            total += value;
        
        if(total == 0.)
        {
            double uniform_prob = 1.0 / aggregated_probs.size();
            
            for (auto& [type, prob] : aggregated_probs)
                prob = uniform_prob;
            
            return;
        }
        
        std::map<ShotType, double> averaged_probs;
        
        for (const auto& [type, value] : aggregated_probs)
            aggregated_probs[type] = value / total;
        
    }
}

void FilmStatistics::aggregateSlidingWindowProbs(std::deque<classification_result> & sliding_window, std::map<ShotType, double> & aggregated_probs)
{
    aggregated_probs.clear();
    
    if(!sliding_window.empty())
    {
        for (const auto& sample : sliding_window)
        {
            for (const auto& [type, prob] : sample.probabilities)
                aggregated_probs[type] += prob;
            
        }
    }
}

void FilmStatistics::findShotTypeMaxProb(std::map<ShotType, double> & probs, classification_result & sample)
{
    auto max_it = std::max_element(
        probs.begin(),
        probs.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });

    if (max_it != probs.end()) {
        ShotType most_likely_type = max_it->first;

        sample.predictedType = most_likely_type;
        sample.probabilities = probs;
    }
    
}

void FilmStatistics::oversampleInputData()
{

    std::map<ShotType, double> aggregated_probs;

    aggregateSlidingWindowProbs(oversampling_sliding_window, aggregated_probs);
    normalizeProbs(aggregated_probs);
    findShotTypeMaxProb(aggregated_probs, current_sample_oversampled);
   
}

void FilmStatistics::computeEntropy()
{

    std::map<ShotType, double> aggregated_probs;

    aggregateSlidingWindowProbs(entropy_sliding_window, aggregated_probs);
    normalizeProbs(aggregated_probs);

    entropy = 0.;

    for (const auto& [type, prob] : aggregated_probs) {
        if (prob > 0.0) {
            entropy -= prob * std::log2(prob);
        }
    }
}

void FilmStatistics::computeEntropyVariance()
{
    entropy_variance_sliding_window.push_back(entropy);
    
    if(entropy_variance_sliding_window.size() > config.entropy_variance_window_size)
        entropy_variance_sliding_window.pop_front();

    if(entropy_variance_sliding_window.size() <= 1)
    {
        entropy_variance = 0;
        return;
    }
        
    
    double mean_entropy = std::accumulate(entropy_variance_sliding_window.begin(), entropy_variance_sliding_window.end(), 0.0) / entropy_variance_sliding_window.size();

    for (const auto& e : entropy_variance_sliding_window) {
        entropy_variance += (e - mean_entropy) * (e - mean_entropy);
    }

        entropy_variance /= (entropy_variance_sliding_window.size() - 1);

}


// main function with all calls
void FilmStatistics::addFrameResult(const double timestampMs, const classification_result& result)
{
    total_frames++;
    
    // startup delay
    if(start_delay == 0)
    {
        size_t max_s = (std::max(config.entropy_variance_window_size, config.entropy_window_size));
        start_delay = static_cast<int>(std::max(max_s, config.input_oversample));
        start_delay *= 2;
    }
    
    if (total_frames%config.input_step == 0)
    {
        evaluated_frames ++;
        
        // compute time needed for one frame -> needed for sync
        if(evaluated_frames == start_delay)
        {
            frame_time_measurement = timestampMs/(start_delay-1);
            start_delay = -start_delay;
        }
            
        timestamp_ms = timestampMs;
        
        oversampling_sliding_window.push_back(result);
        entropy_sliding_window.push_back(result);
        
        
        if(oversampling_sliding_window.size() > config.input_oversample)
            oversampling_sliding_window.pop_front();
        
        if(entropy_sliding_window.size() > config.entropy_window_size)
            entropy_sliding_window.pop_front();
        
        
        oversampleInputData();
        computeEntropy();
        computeEntropyVariance();
        outputStatistics();
        
    }
    
    if(config.output_ratios_series)
       shot_counts[result.predictedType]++;

}



void FilmStatistics::addConfigurationStruct(FilmStatisticsEvalConfig const & cfg)
{
    config = cfg;
}


// --- functions for outputing ---

// calling of all statistic functions
void FilmStatistics::outputStatistics()
{
    if(evaluated_frames > start_delay)
    {
        if(config.output_prob_time_series > 0)
        {
            appendSampleToTimeline(prob_timeline);
            appendSampleToTimeline(shot_type_timeline);
        }
        
        if(config.output_entropy_time_series)
            appendEntropyToTimeline(enthropy_timeline);
        
        if(config.output_entropy_variance_time_series)
            appendEntropyVarianceToTimeline(enthropy_variance_timeline);
        
        if(config.output_cut_detection_time_series)
            appendCutDetectionToTimeline(cut_detection_timeline);
    }
    
}

void FilmStatistics::appendSampleToTimeline(std::vector<std::pair<double, std::map<ShotType, double>>> & timeline)
{
    if(config.output_prob_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_prob_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.input_oversample)/2.);
        
        std::pair<double, std::map<ShotType, double>> sample;
        sample.first = timestamp;
        sample.second = current_sample_oversampled.probabilities;
        
        if(timestamp >= abs(start_delay * frame_time_measurement))
            timeline.push_back(sample);
    }
}

void FilmStatistics::appendSampleToTimeline(std::vector<std::pair<double, ShotType>> & timeline)
{
    if(config.output_prob_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_prob_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.input_oversample)/2.);
        
        std::pair<double, ShotType> sample;
        sample.first = timestamp;
        // Replacement for std::to_string line:
        std::string shotTypeStr;
        switch (current_sample_oversampled.predictedType) {
            case ShotType::CLOSE_UP: shotTypeStr = "CLOSE_UP"; break;
            case ShotType::MEDIUM: shotTypeStr = "MEDIUM"; break;
            case ShotType::WIDE: shotTypeStr = "WIDE"; break;
            default: shotTypeStr = "UNKNOWN"; break;
        }
        sample.second = current_sample_oversampled.predictedType;
        
        if(timestamp >= abs(start_delay * frame_time_measurement))
                timeline.push_back(sample);
    }
}

//size of vector depends on buffer size, it starts from same timestap
void FilmStatistics::appendEntropyToTimeline(std::vector<std::pair<double, double>> & timeline)
{
    if(config.output_entropy_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_entropy_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.entropy_window_size)/2.);
        
        std::pair<double, double> sample;
        sample.first = timestamp;
        sample.second = entropy;
        
        if(timestamp >= abs(start_delay * frame_time_measurement))
            timeline.push_back(sample);
    }
}

void FilmStatistics::appendEntropyVarianceToTimeline(std::vector<std::pair<double, double>> & timeline)
{
    if(config.output_entropy_variance_time_series == 0)
        return;
    
    else if(evaluated_frames%config.output_entropy_variance_time_series == 0)
    {
        double timestamp = timestamp_ms - frame_time_measurement * round(static_cast<double>(config.entropy_variance_window_size)/2.);
        
        std::pair<double, double> sample;
        sample.first = timestamp;
        sample.second = entropy_variance;
        
        if(timestamp >= abs(start_delay * frame_time_measurement))
            timeline.push_back(sample);
    }
}

void FilmStatistics::appendCutDetectionToTimeline(std::vector<std::pair<double, bool>> & timeline)
{
    uint8_t entropy_countdown_counter = 0;
    uint8_t shot_diff_countdown_counter = 0;
    
    if(shot_type_timeline.empty() || enthropy_timeline.empty())
        return;
    
    if(evaluated_frames%config.output_cut_detection_time_series == 0)
    {
        std::pair<double, bool> sample;
        sample.first = shot_type_timeline.back().first;
        
        if(shot_type_timeline.size() < 2 || enthropy_timeline.size() < 2)
        {
            sample.second = false;
            timeline.push_back(sample);
            return;
        }
        
        
        if(enthropy_timeline.back().second > config.cut_detection_entropy_treshold)
            entropy_countdown_counter = config.cut_detection_history_window_size;
        
        if(shot_type_timeline.back().second != shot_type_timeline[shot_type_timeline.size()-2].second)
            shot_diff_countdown_counter = config.cut_detection_history_window_size; // maybe more tresholds will be needed
        
        double entropy_diff = abs(enthropy_timeline[enthropy_timeline.size()-2].second - enthropy_timeline.back().second);
        
        if((shot_diff_countdown_counter > 0 && shot_diff_countdown_counter > 0) || entropy_diff >= config.cut_detection_entropy_diff_treshold)
            sample.second = true;
        else
            sample.second = false;
        
        timeline.push_back(sample);
    }
    return;
    
}

void FilmStatistics::exportToCSV(const std::string& path) const
{
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << path << std::endl;
        return;
    }

    file << "Timestamp,ShotType,Entropy,EntropyVariance,CutDetected";
    // Replacement for loop generating CSV headers:
    for (ShotType type : {ShotType::CLOSE_UP, ShotType::MEDIUM, ShotType::WIDE, ShotType::UNKNOWN}) {
        std::string typeStr;
        switch (type) {
            case ShotType::CLOSE_UP: typeStr = "CLOSE_UP"; break;
            case ShotType::MEDIUM: typeStr = "MEDIUM"; break;
            case ShotType::WIDE: typeStr = "WIDE"; break;
            default: typeStr = "UNKNOWN"; break;
        }
        file << ",Prob_ShotType_" << typeStr;
    }
    file << "\n";

    size_t total_samples = std::min({shot_type_timeline.size(), enthropy_timeline.size(), enthropy_variance_timeline.size(), cut_detection_timeline.size()});

    for (size_t i = 0; i < total_samples; ++i) {
        double timestamp = shot_type_timeline[i].first;
        
        // Replacement for shotTypeStr assignment:
        std::string shotTypeStr;
        switch (shot_type_timeline[i].second) {
            case ShotType::CLOSE_UP: shotTypeStr = "CLOSE_UP"; break;
            case ShotType::MEDIUM: shotTypeStr = "MEDIUM"; break;
            case ShotType::WIDE: shotTypeStr = "WIDE"; break;
            default: shotTypeStr = "UNKNOWN"; break;
        }
        
        double entropy = enthropy_timeline[i].second;
        
        double entropy_variance = enthropy_variance_timeline[i].second;
        
        bool cut_detected = cut_detection_timeline[i].second;

        file << timestamp << "," << shotTypeStr << "," << entropy << "," << entropy_variance << "," << cut_detected;
        if (i < prob_timeline.size()) {
            const auto& probs = prob_timeline[i].second;
            // Replacement for loop writing probabilities:
            for (ShotType type : {ShotType::CLOSE_UP, ShotType::MEDIUM, ShotType::WIDE, ShotType::UNKNOWN}) {
                auto it = probs.find(type);
                double prob = (it != probs.end()) ? it->second : 0.0;
                file << "," << prob;
            }
        } else {
            for (ShotType type : {ShotType::CLOSE_UP, ShotType::MEDIUM, ShotType::WIDE, ShotType::UNKNOWN}) {
                file << ",0.0";
            }
        }
        file << "\n";
    }

    file.close();
    std::cout << "Exported CSV to " << path << std::endl;
}
