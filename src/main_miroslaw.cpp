#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include "FeatureDetector.hpp"
#include "ShotClassifier.hpp"

// Adjust these paths to your folders
const std::string folder0 = "wide";    // images expected classified as 0
const std::string folder1 = "medium";  // expected 1
const std::string folder2 = "closeup"; // expected 2

int SMALLEST = 3000;
int CLOSEUP = 50000;
int EYE_CLOSEUP_THRESHOLD = 2000; // adjust this based on your testing

namespace fs = std::filesystem;

void processFolder(const std::string& folderPath, int expectedLabel,
                   FeatureDetector& frontal_face_detector,
                   FeatureDetector& side_face_detector,
                   FeatureDetector& eye_detector,
                   ShotClassifier& face_classifier)
{
    int totalImages = 0;
    int correctClassifications = 0;

    ShotClassifier eye_classifier(0, EYE_CLOSEUP_THRESHOLD);

    for (const auto& entry : fs::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file()) continue;

        std::string image_path = entry.path().string();
        cv::Mat image = cv::imread(image_path);
        if (image.empty()) {
            std::cerr << "Warning: Could not load image " << image_path << std::endl;
            continue;
        }

        cv::resize(image, image, cv::Size(1280, 720));

        // Detect frontal faces and eyes
        std::vector<cv::Rect> detected_frontal_faces = frontal_face_detector.detect(image);
        std::vector<cv::Rect> detected_eyes = eye_detector.detect(image);

        // Detect left profile faces (original image)
        std::vector<cv::Rect> detected_side_faces_left = side_face_detector.detect(image);

        // Detect right profile faces by flipping the image horizontally
        cv::Mat flipped_image;
        cv::flip(image, flipped_image, 1); // flip around y-axis

        std::vector<cv::Rect> detected_side_faces_right_flipped = side_face_detector.detect(flipped_image);

        // Convert flipped detections back to original coordinates
        std::vector<cv::Rect> detected_side_faces_right;
        for (const auto& rect : detected_side_faces_right_flipped) {
            // x in original image = width - (x + width) in flipped image
            int x = image.cols - (rect.x + rect.width);
            detected_side_faces_right.emplace_back(cv::Rect(x, rect.y, rect.width, rect.height));
        }

        // Combine left and right profile detections
        std::vector<cv::Rect> detected_side_faces = detected_side_faces_left;
        detected_side_faces.insert(detected_side_faces.end(), detected_side_faces_right.begin(), detected_side_faces_right.end());

        // Combine all faces
        std::vector<cv::Rect> all_detected_faces = detected_frontal_faces;
        all_detected_faces.insert(all_detected_faces.end(), detected_side_faces.begin(), detected_side_faces.end());

        // Classify
        int classification_result = face_classifier.classify(all_detected_faces);

        if (classification_result == 1 || classification_result == 2) {
            if (eye_classifier.classify(detected_eyes) == 2) {
                classification_result = 2;
            }
        }

        if (classification_result == expectedLabel)
            ++correctClassifications;

        ++totalImages;

        // Logging
        std::cout << "Image: " << image_path << std::endl;
        std::cout << "  Frontal Faces: " << detected_frontal_faces.size() << std::endl;
        std::cout << "  Profile Faces: " << detected_side_faces.size() << std::endl;
        std::cout << "  Eyes: " << detected_eyes.size() << std::endl;

        std::vector<cv::Rect> all_boxes = all_detected_faces;
        if (!all_boxes.empty()) {
            int max_area = all_boxes.front().area();
            int min_area = all_boxes.front().area();
            for (const auto& box : all_boxes) {
                int area = box.area();
                if (area > max_area) max_area = area;
                if (area < min_area) min_area = area;
            }
            std::cout << "  Largest Box Area (Face or Eye): " << max_area << std::endl;
            std::cout << "  Smallest Box Area (Face or Eye): " << min_area << std::endl;
        } else {
            std::cout << "  No faces or eyes detected." << std::endl;
        }

        std::cout << "  Classification Result: " << classification_result << "\n\n";
    }

    std::cout << "Folder: " << folderPath << std::endl;
    std::cout << "Total images processed: " << totalImages << std::endl;
    std::cout << "Correct classifications: " << correctClassifications << std::endl;
    std::cout << "Accuracy: " << (totalImages ? (100.0 * correctClassifications / totalImages) : 0) << "%\n\n";
}

int main() {
    std::string frontal_path = "haarcascade_frontalface_alt2.xml";
    std::string profile_path = "haarcascade_profileface.xml";
    std::string eye_path = "haarcascade_eye.xml";

    FeatureDetector frontal_detector(frontal_path);
    FeatureDetector profile_detector(profile_path);
    FeatureDetector eye_detector(eye_path);

    ShotClassifier face_classifier(SMALLEST, CLOSEUP);

    processFolder(folder0, 0, frontal_detector, profile_detector, eye_detector, face_classifier);
    processFolder(folder1, 1, frontal_detector, profile_detector, eye_detector, face_classifier);
    processFolder(folder2, 2, frontal_detector, profile_detector, eye_detector, face_classifier);

    return 0;
}
