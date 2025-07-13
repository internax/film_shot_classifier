//#include <opencv2/opencv.hpp>
//#include <iostream>
////#include <opencvi2/highgui.hpp>
////#include <opencv2/imgproc.hpp>
//#include <filesystem>
////#include "FilmShotClassifier.hpp"
//#include "FileLoader.hpp"
//
//#include <vector>
//#include <random>
//#include <map>
//#include <iostream>
//#include "UserStructs.hpp"
//
//#include "FileLoader.hpp"
//#include "HaarDetector.hpp"
//#include "ShotClassifier.hpp"
//#include "ShotEvaluator.hpp"
//#include "UserStructs.hpp"
//#include "FilmStatisticEval.hpp"
//
//#include <chrono>
//
//////std::vector<ClassificationResult> generateRandomResults(size_t count = 100) {
//////    std::vector<ClassificationResult> results;
//////    std::random_device rd;
//////    std::mt19937 gen(rd());
//////    std::uniform_real_distribution<double> dist(0.0, 1.0);
//////    std::uniform_int_distribution<int> typeDist(0, 2); // CLOSE_UP, MEDIUM, WIDE
//////
//////    ShotType current_type = static_cast<ShotType>(typeDist(gen));
//////
//////    for (size_t i = 0; i < count; ++i) {
//////        if (i % 10 == 0) {
//////            current_type = static_cast<ShotType>(typeDist(gen));
//////        }
//////
//////        ClassificationResult res;
//////        res.predictedType = current_type;
//////
//////        // generate probabilities for all 3 types
//////        std::map<ShotType, double> raw_probs = {
//////            {ShotType::CLOSE_UP, dist(gen)},
//////            {ShotType::MEDIUM, dist(gen)},
//////            {ShotType::WIDE, dist(gen)}
//////        };
//////
//////        // force the predicted type to be the highest, with stronger dominance
//////        raw_probs[current_type] += 3.0;
//////
//////        // normalize
//////        double total = 0;
//////        for (const auto& p : raw_probs) total += p.second;
//////        for (const auto& p : raw_probs)
//////            res.probabilities[p.first] = p.second / total;
//////
//////        results.push_back(res);
//////    }
//////
//////    return results;
//////}
//////
//////std::vector<double> generateTimestamps(size_t count, double step = 100.0) {
//////    std::vector<double> timestamps;
//////    timestamps.reserve(count);
//////    for (size_t i = 0; i < count; ++i) {
//////        timestamps.push_back(i * step);
//////    }
//////    return timestamps;
//////}
//////
/////
//
//bool isSceneChanged(const cv::Mat& prev, const cv::Mat& curr, double threshold = 0.5, int pixelDiffThreshold = 60)
//{
//    if (prev.empty() || curr.empty() || prev.size() != curr.size()) {
//        return false; // nesrovnatelné snímky
//    }
//
//    cv::Mat gray1, gray2;
//    
//    if (prev.channels() == 3)
//        cv::cvtColor(prev, gray1, cv::COLOR_BGR2GRAY);
//    else
//        gray1 = prev.clone();
//
//    if (curr.channels() == 3)
//        cv::cvtColor(curr, gray2, cv::COLOR_BGR2GRAY);
//    else
//        gray2 = curr.clone();
//    cv::Mat diff, thresh;
//    cv::absdiff(gray1, gray2, diff);
//    cv::threshold(diff, thresh, pixelDiffThreshold, 255, cv::THRESH_BINARY);
//
//    double changedPixels = cv::countNonZero(thresh);
//    double totalPixels = gray1.rows * gray1.cols;
//    double changeRatio = changedPixels / totalPixels;
//
//    return changeRatio >= threshold;
//}
//
//
//constexpr int optimizer_distance_between_samples = 3;
//
//int constexpr WIDE_THRESHOLD = 3000;
//int constexpr CLOSEUP_THRESHOLD = 50000;
//int constexpr EYE_THRESHOLD = 2000;
//
//int main()
//{
//    auto start_time = std::chrono::high_resolution_clock::now();
//
//    cv::Mat frame;
//    cv::Mat processed;
//    cv::Mat previous;
//    
//   
//
//    // Load all images from the directory
//    VideoLoader image_loader("video/marvel.mp4");
//
//    // Haar detector paths
//    std::string frontal_path = "haar/haarcascade_frontalface_alt2.xml";
//    std::string profile_path = "haar/haarcascade_profileface.xml";
//    std::string eye_path = "haar/haarcascade_eye.xml";
//
//    // Initialize detectors and classifier
//    HaarDetector frontal_detector(frontal_path);
//    HaarDetector profile_detector(profile_path);
//    HaarDetector eye_detector(eye_path);
//    
//    ShotClassifier face_classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
//    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, face_classifier, EYE_THRESHOLD);
//
//    FilmStatisticsEvalConfig stat_cfg;
//    FilmStatistics stats;
//    stats.addConfigurationStruct(stat_cfg);
//    
//    ClassificationResult classification_result;
//    
//    int counter_proc = 0;
//    int counter_all = 0;
//    int counter = 1;
//    
//    std::vector<cv::Rect> faces, eyes;
//    frame = image_loader.nextFrame();
//    classification_result = evaluator.evaluate(frame, faces, eyes);
//    
//
//    std::deque<cv::Mat> frame_history;
//    
//    
//    for(int i = optimizer_distance_between_samples; i > 0; --i)
//    {
//        frame_history.push_back(frame);
//    }
//    
//    
//    while (image_loader.hasNextFrame())
//    {
//        frame.release();
//        frame = image_loader.nextFrame();
//
//        // Preprocess the frame
//        Preprocessing processor;
//        processor.LoadFrame(frame);
//        processed = processor.GetProcessedImage();
//
//        if (processed.empty()) {
//            std::cerr << "Error: Processed frame is empty." << std::endl;
//            continue;
//        }
//
//        
//        
//        
////        if(counter%10 == 0)
////        {
////            classification_result = evaluator.evaluate(processed, faces, eyes);
////            counter_proc ++;
////            counter = 1;
////        }
////        else
////            counter++;
//        
//        
//        if(isSceneChanged(frame_history.front(), processed))
//        {
//            classification_result = evaluator.evaluate(processed, faces, eyes);
//            counter_proc ++;
//        }
//        
//        counter_all ++;
//        stats.addFrameResult(image_loader.getCurrentTimestamp(), classification_result);
//        
//        frame_history.pop_front();
//        frame_history.push_back(processed);
//        previous = processed;
///*
//        // Draw bounding boxes
//        for (const auto& face : faces) {
//            cv::rectangle(processed, face, cv::Scalar(0, 255, 0), 2);
//        }
//
//        for (const auto& eye : eyes) {
//            cv::rectangle(processed, eye, cv::Scalar(255, 0, 0), 2);
//        }
//
//        // Show processed frame with annotations
//        cv::imshow("Processed Image", processed);
//        cv::waitKey(2000);
//        cv::destroyWindow("Processed Image");
//*/
//    }
//
//    stats.exportToCSV("csv_output/output_video_marvel_opt30_25.csv");
//    
//    auto end_time = std::chrono::high_resolution_clock::now();
//    double elapsed = std::chrono::duration<double>(end_time - start_time).count();
//
//    double ratio = static_cast<double>(counter_proc) / static_cast<double>(counter_all);
//    std::cout << "\n| Processed Frames | Total Frames | Ratio     | Runtime (s) |\n";
//    std::cout <<   "|------------------|--------------|-----------|-------------|\n";
//    std::cout <<   "| " << counter_proc << "               | " << counter_all << "           | " << ratio << "    | ";
//    std::cout << elapsed << "      |\n";
//
//
//
//    return 0;
//}
//
////    cv::Mat image;
////
////    ImageLoader image_loader("../test/closeup");
////
////    while(image_loader.hasNextFrame())
////    {
////        image.release();
////        image = image_loader.nextFrame();
////        cv::imshow("img", image);
////        cv::waitKey(500);
////        cv::destroyWindow("img");
////    }
////
////    VideoLoader video_loader ("../video/SampleVideo.mp4");
////
////    while(video_loader.hasNextFrame())
////    {
////        image.release();
////        image = video_loader.nextFrame();
////        cv::imshow("img", image);
////        cv::waitKey(10);
////        cv::destroyWindow("img");
////    }
////
////
//////// just a code prototype, comment out to build
//////    std::string data_path = "path";
//////    std::string haar_filter_path1 = "path";
//////    std::string haar_filter_path2 = "path";
////
//////    ImageLoader image_loader(data_path);
//////    Preprocessing preprocess;
//////    FeatureDetector frontal_face_detector(haar_filter_path1);
//////    FeatureDetector side_face_detector(haar_filter_path2);
//////    ShotFeatureExtractor shot_feature_extractor;
//////    ShotClassifier shot_classifier;
//////    FilmStatistics film_stats;
//////
//////    std::vector<DetectedFeature> detected_frontal_face_vect;
//////    std::vector<DetectedFeature> detected_left_face_vect;
//////    std::vector<DetectedFeature> features_vect;
//////
//////    ShotFeatures shot_features;
//////    ClassificationResult classification_result;
//////
//////    while(image_loader.hasNextFrame())
//////    {
//////        preprocess.LoadFrame(image_loader.nextFrame());
//////        //preprocessing methods calling...
//////
//////        detected_frontal_face_vect = frontal_face_detector.detect(preprocess.GetProcessedImage());
//////        detected_left_face_vect = side_face_detector.detect(preprocess.GetProcessedImage());
//////        // concatenate vectors... -> features_vect
//////
//////        shot_features = shot_feature_extractor.extract(preprocess.GetProcessedImage(), features_vect);
//////        classification_result = shot_classifier.classify(shot_features);
//////        film_stats.addFrameResult(image_loader.getCurrentTimestamp(), classification_result);
//////    }
////
////
////  // Ujisti se, že soubor je dostupný
////
////
////  /*
////
////        // Vygeneruj náhodná data
////    std::vector<classification_result> simulated_results = generateRandomResults(100);
////        std::vector<double> simulated_timestamps = generateTimestamps(100, 100.0);
////
////        // Výpis pro kontrolu (například prvních 5)
////        for (size_t i = 0; i < 5; ++i) {
////            std::cout << "Timestamp: " << simulated_timestamps[i] << " ms, Predicted: ";
////            switch (simulated_results[i].predictedType) {
////                case ShotType::CLOSE_UP: std::cout << "CLOSE_UP"; break;
////                case ShotType::MEDIUM: std::cout << "MEDIUM"; break;
////                case ShotType::WIDE: std::cout << "WIDE"; break;
////                default: std::cout << "UNKNOWN"; break;
////            }
////            std::cout << ", Probabilities: ";
////            for (const auto& [type, prob] : simulated_results[i].probabilities) {
////                std::cout << static_cast<int>(type) << ":" << prob << " ";
////            }
////            std::cout << std::endl;
////        }
////
////    FilmStatistics film_stats;
////
////    FilmStatisticsEvalConfig cfg;
////    film_stats.addConfigurationStruct(cfg);
////
////    for (size_t i = 0; i < simulated_results.size(); ++i) {
////        film_stats.addFrameResult(simulated_timestamps[i], simulated_results[i]);
////    }
////
////    std::vector<std::pair<double, double>> ent = film_stats.getEntropy();
////    std::vector<std::pair<double, double>> ent_v = film_stats.getEntropyVAriance();
////    std::vector<std::pair<double, ShotType>> st = film_stats.getShotType();
////
////    film_stats.exportToCSV("path");
////
////   */
////
////
////
////
////
////
////
////
////
////
