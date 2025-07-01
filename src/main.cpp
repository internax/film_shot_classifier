#include <opencv2/opencv.hpp>
#include <iostream>
//#include <opencvi2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <filesystem>
#include "FilmShotClassifier.hpp"


#include <vector>
#include <random>
#include <map>
#include <iostream>
#include "UserStructs.hpp"

std::vector<classification_result> generateRandomResults(size_t count = 100) {
    std::vector<classification_result> results;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::uniform_int_distribution<int> typeDist(0, 2); // CLOSE_UP, MEDIUM, WIDE

    ShotType current_type = static_cast<ShotType>(typeDist(gen));

    for (size_t i = 0; i < count; ++i) {
        if (i % 10 == 0) {
            current_type = static_cast<ShotType>(typeDist(gen));
        }

        classification_result res;
        res.predictedType = current_type;

        // generate probabilities for all 3 types
        std::map<ShotType, double> raw_probs = {
            {ShotType::CLOSE_UP, dist(gen)},
            {ShotType::MEDIUM, dist(gen)},
            {ShotType::WIDE, dist(gen)}
        };

        // force the predicted type to be the highest, with stronger dominance
        raw_probs[current_type] += 3.0;

        // normalize
        double total = 0;
        for (const auto& p : raw_probs) total += p.second;
        for (const auto& p : raw_probs)
            res.probabilities[p.first] = p.second / total;

        results.push_back(res);
    }

    return results;
}

std::vector<double> generateTimestamps(size_t count, double step = 100.0) {
    std::vector<double> timestamps;
    timestamps.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        timestamps.push_back(i * step);
    }
    return timestamps;
}

int main()
{
// just a code prototype, comment out to build
    std::string data_path = "path";
    std::string haar_filter_path1 = "path";
    std::string haar_filter_path2 = "path";
    
//    ImageLoader image_loader(data_path);
//    Preprocessing preprocess;
//    FeatureDetector frontal_face_detector(haar_filter_path1);
//    FeatureDetector side_face_detector(haar_filter_path2);
//    ShotFeatureExtractor shot_feature_extractor;
//    ShotClassifier shot_classifier;
//    FilmStatistics film_stats;
//    
//    std::vector<DetectedFeature> detected_frontal_face_vect;
//    std::vector<DetectedFeature> detected_left_face_vect;
//    std::vector<DetectedFeature> features_vect;
//    
//    ShotFeatures shot_features;
//    ClassificationResult classification_result;
//    
//    while(image_loader.hasNextFrame())
//    {
//        preprocess.LoadFrame(image_loader.nextFrame());
//        //preprocessing methods calling...
//        
//        detected_frontal_face_vect = frontal_face_detector.detect(preprocess.GetProcessedImage());
//        detected_left_face_vect = side_face_detector.detect(preprocess.GetProcessedImage());
//        // concatenate vectors... -> features_vect
//        
//        shot_features = shot_feature_extractor.extract(preprocess.GetProcessedImage(), features_vect);
//        classification_result = shot_classifier.classify(shot_features);
//        film_stats.addFrameResult(image_loader.getCurrentTimestamp(), classification_result);
//    }
    
    
  // Ujisti se, že soubor je dostupný

   
        // Vygeneruj náhodná data
    std::vector<classification_result> simulated_results = generateRandomResults(100);
        std::vector<double> simulated_timestamps = generateTimestamps(100, 100.0);

        // Výpis pro kontrolu (například prvních 5)
        for (size_t i = 0; i < 5; ++i) {
            std::cout << "Timestamp: " << simulated_timestamps[i] << " ms, Predicted: ";
            switch (simulated_results[i].predictedType) {
                case ShotType::CLOSE_UP: std::cout << "CLOSE_UP"; break;
                case ShotType::MEDIUM: std::cout << "MEDIUM"; break;
                case ShotType::WIDE: std::cout << "WIDE"; break;
                default: std::cout << "UNKNOWN"; break;
            }
            std::cout << ", Probabilities: ";
            for (const auto& [type, prob] : simulated_results[i].probabilities) {
                std::cout << static_cast<int>(type) << ":" << prob << " ";
            }
            std::cout << std::endl;
        }

    FilmStatistics film_stats;
    
    FilmStatisticsEvalConfig cfg;
    film_stats.addConfigurationStruct(cfg);

    for (size_t i = 0; i < simulated_results.size(); ++i) {
        film_stats.addFrameResult(simulated_timestamps[i], simulated_results[i]);
    }
    
    std::vector<std::pair<double, double>> ent = film_stats.getEntropy();
    std::vector<std::pair<double, double>> ent_v = film_stats.getEntropyVAriance();
    std::vector<std::pair<double, ShotType>> st = film_stats.getShotType();
    
    film_stats.exportToCSV("path");
        
    return 0;
}
