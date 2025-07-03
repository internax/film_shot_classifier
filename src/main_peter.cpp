#include "../include/FileLoader.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " <path_to_image_or_video>" << std::endl;
    //     return -1;
    // }
    // display_media(argv[1]);

    //TEST THE IMAGE LOADER
    //std::string test_image_path = "/Users/pobaloluwa/Documents/film_shot_classifier/test/wide/6.jpg"; 
    //cv::Mat image = load_image(test_image_path);
    // cv::imshow("Loaded Image", image);
    // cv::waitKey(0);


    //display_media(test_image_path); //or use this to display


    //TEST THE VIDEO LOADER
    std::string test_video_path = "/Users/pobaloluwa/Downloads/mobile.MP4";

    std::vector<cv::Mat> frames = load_video_frames(test_video_path);
   
   for (const auto& frame : frames) {
        cv::imshow("Loaded Video", frame);
        if (cv::waitKey(30) == 27) break; // 30 ms delay, exit on 'ESC'
    }    

    //display_media(test_video_path); //or use this to display
    return 0;
}
