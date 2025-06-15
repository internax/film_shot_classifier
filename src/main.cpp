#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "FilmShotClassifier.hpp"
#include "FileLoader.hpp"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_image>" << std::endl;
        return -1;
    }

    try {

        // Use the command line argument as the image path
        ImageLoader loader(argv[1]);
        
        // Check if there's a frame available
        if (loader.hasNextFrame()) {
            std::cout << "Frame is available" << std::endl;
            
            // Get the frame
            cv::Mat frame = loader.nextFrame();
            
            if (frame.empty()) {
                std::cout << "Error: Frame is empty" << std::endl;
                return -1;
            }
            
            // Create a preprocessing instance
            Preprocessing processor;
            processor.LoadFrame(frame);
            
            // Get the processed frame
            cv::Mat processed = processor.GetProcessedImage();

            std::cout << "Successfully resized frame to: " << processed.size().width 
                      << "x" << processed.size().height << std::endl;
            
            // Display the image (optional)
            cv::imshow("Loaded Image", processed);
            cv::waitKey(0);
        } else {
            std::cout << "No frame available" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
