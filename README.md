# 🎬 Film Shot Type Classification

## 📅 Timeline

| Activity                            | Deadline       |
|-------------------------------------|----------------|
| Project proposal & test samples     | 19 May         |
| Proposal discussion (lab session)   | 20 May         |
| Finalization & approval             | 26 May         |

This project focuses on the automatic classification of cinematic shot types (such as **close-up**, **medium**, and **wide** shots) from video or image data. The classification is based on detecting and analyzing visual features — primarily faces and objects — using classical computer vision techniques (e.g., **Haar cascades**) and handcrafted feature extraction.

The motivation for this tool is to support tasks such as:
- **Film style analysis**
- **Shot distribution statistics**
- **Automated metadata generation** for video datasets

The application processes input frame-by-frame, extracts spatial and geometric features, classifies the shot type, and optionally evaluates the performance against a labeled dataset.

---

## 📂 Tasks
- `main` – Program entry point 
- `UserStructs` – Definitions of shared data types and enums

###Miroslaw
- `FeatureDetector` – Object detection using Haar cascades  
- `FeatureProcessorAndClassifier` – Feature extraction and shot type classification logic  

###Peter
- `FileLoader` – Abstract interface for loading images or video  
- `ResultDisplayer` – Displays or exports results  

###Marek 
- `FilmStatisticEval` – Aggregates classification results across frames  
- `TestDatasetEval` – Compares predictions with ground truth labels  

# 📄 Final Project Report 
*Here is report structure derived from example project in moodle*

## 1. Title Page
- **Project title**: Film Shot Type Classification  
- **Course**: Computer Vision  
- **Authors**: [Your Name(s)]  
- **Submission Date**: [Insert date]  

---

## 2. Introduction
- Brief background on the problem domain (e.g., film analysis, automatic metadata generation)  
- Motivation for choosing this topic  
- Main objectives and intended use of the system  

---

## 3. System Overview
- High-level description of the system pipeline  
- Diagram or figure (optional)  
- Brief description of each major module:
  - Input loading (image/video)
  - Feature detection
  - Feature extraction
  - Shot classification
  - Statistical analysis
  - Output/display module

---

## 4. Implementation Details
- Technical breakdown of each module  
- Algorithms and techniques used (e.g., Haar cascades, OpenCV features)  
- Programming language and tools used  
- Code structure and how different parts interact  

---

## 5. Evaluation

### Quantitative Evaluation
- Accuracy, confusion matrix, or class-wise performance  
- Dataset size and statistics  

### Qualitative Evaluation
- Visual examples:
  - Input frame(s)
  - Detected objects
  - Predicted shot type
  - Statistical output (timeline, chart, etc.)

---

## 6. Dataset Description
- Source of the dataset (test images, videos, ground truth)  
- Label definitions (e.g., CLOSE_UP, MEDIUM, WIDE)  
- Preprocessing steps applied  
- Any augmentations or synthetic data used  

---

## 7. Results Summary
- General observations  
- Performance comparison if multiple methods were tested  
- Strengths and weaknesses of the current implementation  

---

## 8. Individual Contributions

| Team Member | Contribution                                  | Hours |
|-------------|-----------------------------------------------|--------|
| Member 1    | e.g., feature detection, classifier module     |        |
| Member 2    | e.g., evaluation, testing, visualization       |        |

---

## 9. Conclusion
- Final thoughts and achievements  
- Summary of results and project goals met  
- Ideas for improvement or future extensions  

---

## 10. References
- Articles, documentation, libraries used (e.g., OpenCV)  
- Any relevant tutorials, academic papers, GitHub repos  
