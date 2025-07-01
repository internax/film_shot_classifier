cmake_minimum_required(VERSION 3.10)
project(film_shot_classifier)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include directories
include_directories(/opt/homebrew/opt/opencv/include/opencv4)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add source files
set(SOURCES
    src/FileLoader.cpp
    src/FeatureDetector.cpp
    src/FeatureProccesorAndClassifier.cpp
    src/FilmStatisticEval.cpp
    src/ResultDisplayer.cpp
    src/main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}) 