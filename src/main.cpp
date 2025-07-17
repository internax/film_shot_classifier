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
//#include <chrono>
//
//#include "FileLoader.hpp"
//#include "HaarDetector.hpp"
//#include "ShotClassifier.hpp"
//#include "ShotEvaluator.hpp"
//#include "UserStructs.hpp"
//#include "FilmStatisticEval.hpp"
//#include "SceneChangeDetect.hpp"
//#include "ResultDisplayer.hpp"
//
//
//constexpr int OPTIMIZER_DISTANCE_BETWEEN_SAMPLES = 3;
//
//constexpr int RESOLUTION_WIDTH = 1280;
//constexpr int RESOLUTION_HEIGHT = 720;
//
//constexpr int PIXEL_DIFF_TRESHOLD = 60;
//constexpr double IMAGE_CHANGE_RATIO = 0.5;
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
//
//    //VideoLoader class + preprocessor init
//    VideoLoader image_loader("video/marvel.mp4");
//    Preprocessing processor;
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
//    //Viola & Johnes classifier init
//    ShotClassifier face_classifier(WIDE_THRESHOLD, CLOSEUP_THRESHOLD);
//    ShotEvaluator evaluator(frontal_detector, profile_detector, eye_detector, face_classifier, EYE_THRESHOLD);
//    ClassificationResult classification_result;
//    std::vector<cv::Rect> faces, eyes;
//
//    //Statistic class init
//    FilmStatisticsEvalConfig stat_cfg;
//    FilmStatistics stats;
//    stats.addConfigurationStruct(stat_cfg);
//    
//    //History queue for optimizer
//    std::deque<cv::Mat> frame_history;
//    for(int i = OPTIMIZER_DISTANCE_BETWEEN_SAMPLES; i > 0; --i)
//        frame_history.push_back(frame);
//    
//    //Counters for statstic
//    int counter_proc = 0;
//    int counter_all = 0;
//
//    //Clasification of the first frame
//    frame = image_loader.nextFrame();
//    classification_result = evaluator.evaluate(frame, faces, eyes);
//    
//    while (image_loader.hasNextFrame())
//    {
//        frame.release();
//        frame = image_loader.nextFrame();
//
//        processor.LoadFrame(frame);
//        processor.resizeImage(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
//        processor.equalizeHistogram();
//        frame = processor.GetProcessedImage();
//
//        if( isSceneChanged(frame_history.front(), frame, IMAGE_CHANGE_RATIO, PIXEL_DIFF_TRESHOLD))
//        {
//            classification_result = evaluator.evaluate(frame, faces, eyes);
//            counter_proc++;
//        }
//        
//        counter_all ++;
//        stats.addFrameResult(image_loader.getCurrentTimestamp(), classification_result);
//        
//        frame_history.pop_front();
//        frame_history.push_back(frame);
//    }
//
//    stats.exportToCSV("csv_output/output_video_marvel_opt50_60.csv");
//    
//    auto end_time = std::chrono::high_resolution_clock::now();
//    double elapsed = std::chrono::duration<double>(end_time - start_time).count();
//
//    //print table in md format
//    double ratio = static_cast<double>(counter_proc) / static_cast<double>(counter_all);
//    std::cout << "\n| Processed Frames | Total Frames | Ratio     | Runtime (s) |\n";
//    std::cout <<   "|------------------|--------------|-----------|-------------|\n";
//    std::cout <<   "| " << counter_proc << "              | " << counter_all << "         |" << ratio << "  | ";
//    std::cout << elapsed << "     |\n\n";
//
//    stats.printSummary();
//    
//    
////    ResultDisplayer displayer(stats);
////    cv::Mat plot = displayer.GetPlot(stats.getProbTimeline(), "x", "y", "graph");
////    cv::imshow("plot", plot);
////    cv::waitKey(0);
//
//    return 0;
//}
