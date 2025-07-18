//
//  FileLoader.hpp
//  Film_type_classifier
//
//  Developed by Marek Tatýrek
//

#ifndef FileLoader_hpp
#define FileLoader_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>
#include <vector>

/**
 * @class InputSource
 * @brief Abstract base class for loading image or video input in a unified way.
 *
 * This class defines a common interface for different input sources (image or video),
 * allowing the rest of the application to interact with frames in a uniform manner.
 * It supports checking for availability of the next frame, retrieving the frame,
 * and (optionally) querying its timestamp.
 *
 * Designed to make it easy to switch between static image input and sequential video input.
 */
class InputSource
{
protected:
    std::string source_path; ///< Path to the input source file (image or video)

public:
    
    
    /**
     * @brief Constructs an input source with a file path.
     * @param path Path to the image or video file.
     */
    explicit InputSource(const std::string& path) : source_path(path) {}

    /**
     * @brief Virtual destructor for polymorphic use.
     */
    virtual ~InputSource() = default;

    /**
     * @brief Checks whether there is a next frame to process.
     * @return True if a frame is available, false otherwise.
     */
    virtual bool hasNextFrame() const = 0;

    /**
     * @brief Returns the next frame.
     * @return A reference to the next `cv::Mat` frame.
     */
    virtual cv::Mat& nextFrame() = 0;

    /**
     * @brief Returns the current timestamp of the frame (in ms).
     * @return Timestamp in milliseconds.
     */
    virtual double getCurrentTimestamp() const = 0;
};





/**
 * @class ImageLoader
 * @brief Concrete implementation of InputSource for loading a single image.
 *
 * Provides a way to treat a single image file as a frame source.
 * Once the image is returned, subsequent calls indicate no more frames are available.
 */
class ImageLoader : public InputSource
{
private:
    cv::Mat image; // Loaded image
    bool loaded = false;
    bool frameReturned = false;
    
    std::vector<std::string> image_paths;
    int current_frame_index = 0;    // reset indexu
    
    
    void loadImagePathsFromDirectory(const std::string& directory);
    
public:
    
    using InputSource::InputSource;

    ImageLoader(const std::string& path) : InputSource(path) 
    {
        loadImagePathsFromDirectory(path);
    }
    
    /**
     * @brief Checks if the image is still available to be returned.
     * @return True if the image hasn't been returned yet.
     */
    bool hasNextFrame() const override;

    /**
     * @brief Returns the loaded image.
     * @return Reference to the loaded image (`cv::Mat`).
     */
    cv::Mat& nextFrame() override;

    /**
     * @brief Returns the timestamp of the image (usually 0).
     * @return Fixed timestamp value (e.g., 0 ms).
     */
    double getCurrentTimestamp() const override;
};

/**
 * @class VideoLoader
 * @brief Placeholder for a video-based implementation of InputSource.
 *
 * This class will provide functionality to iterate over frames from a video file.
 * Not implemented yet.
 */
class VideoLoader : public InputSource
{
    std::string video_path;
    cv::VideoCapture video;
    
    cv::Mat current_frame;
    
    int current_frame_index;
    
    void openVideoFromPath(const std::string & path);
    
public:
    using InputSource::InputSource;
    
    VideoLoader(const std::string& path) : InputSource(path)
    {
        openVideoFromPath(path);
    }
    
    /**
     * @brief Checks if the image is still available to be returned.
     * @return True if the image hasn't been returned yet.
     */
    bool hasNextFrame() const override;

    /**
     * @brief Returns the loaded image.
     * @return Reference to the loaded image (`cv::Mat`).
     */
    cv::Mat& nextFrame() override;

    /**
     * @brief Returns the timestamp of the image (usually 0).
     * @return Fixed timestamp value (e.g., 0 ms).
     */
    double getCurrentTimestamp() const override;
    
    
};









/**
 * @class Preprocessing
 * @brief Class responsible for applying preprocessing operations to a frame.
 *
 * This class loads a frame and provides an interface to process it (e.g., grayscale,
 * histogram equalization, resizing, denoising, etc.).
 *
 * Intended to be extended with actual image preprocessing methods for analysis or detection.
 */
class Preprocessing
{
    cv::Mat image; ///< Internal image being processed

public:
    /**
     * @brief Constructs the preprocessing unit with an input frame.
     * @param frame Frame to preprocess.
     */
    explicit Preprocessing(const cv::Mat frame) : image(frame) {}

    /**
     * @brief Default constructor.
     */
    explicit Preprocessing() {}

    /**
     * @brief Destructor.
     */
    ~Preprocessing();

    /**
     * @brief Loads a new frame into the processor.
     * @param image Frame to load for processing.
     */
    void LoadFrame(cv::Mat& image);
    
    /**
     * @brief Resizes the internal image to the specified dimensions.
     * @param rows Desired number of rows (height).
     * @param cols Desired number of columns (width).
     */
    void resizeImage(int const rows, int const cols);
    /**
     * @brief Converts the internal image to grayscale.
     */
    void toGrayscale();
    /**
     * @brief Applies histogram equalization to enhance image contrast.
     */
    void equalizeHistogram();

    /**
     * @brief Returns the processed image.
     * @return Reference to the processed image.
     */
    cv::Mat& GetProcessedImage();

    // Future: add methods for specific preprocessing steps (blur, resize, etc.)
};

#endif /* FileLoader_hpp */
