cmake_minimum_required(VERSION 3.10)
project(FilmShotClassifier)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(
    src
    include
    ${OpenCV_INCLUDE_DIRS}
)

set(SOURCES
    src/FileLoader.cpp
    src/FilmStatisticEval.cpp
    src/HaarDetector.cpp
    src/SceneChangeDetect.cpp
    src/ShotClassifier.cpp
    src/ShotEvaluator.cpp
    src/TestDatasetEval.cpp
    src/UserStructs.cpp
)

add_executable(main_eval src/main_eval.cpp ${SOURCES})
target_link_libraries(main_eval ${OpenCV_LIBS})

add_executable(main_image_folder src/main_image_folder.cpp ${SOURCES})
target_link_libraries(main_image_folder ${OpenCV_LIBS})

add_executable(main_video src/main_video.cpp ${SOURCES})
target_link_libraries(main_video ${OpenCV_LIBS})
