//
//  ResultDisplayer.cpp
//  Film_type_classifier
//
//  Created by Marek Tatýrek on 21.05.2025.
//

#include "ResultDisplayer.hpp"


cv::Mat ResultDisplayer::GetPlot(std::vector<std::pair<double, std::map<ShotType, double>>> & data_series, std::string xlabel, std::string ylabel, std::string title)
{
    const int width = 800;
    const int height = 600;
    const int margin = 50;
    
    cv::Mat plot(height, width, CV_8UC3, cv::Scalar(255, 255, 255));


    double x_min = data_series.front().first;
    double x_max = data_series.back().first;

    double y_min = DBL_MAX;
    double y_max = -DBL_MAX;

    for (const auto& [x, shot_map] : data_series) {
        for (const auto& [shot, y] : shot_map) {
            y_min = std::min(y_min, y);
            y_max = std::max(y_max, y);
        }
    }

    if (y_min == y_max) y_max += 1.0;

    std::map<ShotType, cv::Scalar> color_map = {
        {ShotType::CLOSE_UP, cv::Scalar(255, 0, 0)},
        {ShotType::MEDIUM,   cv::Scalar(0, 255, 0)},
        {ShotType::WIDE,     cv::Scalar(0, 0, 255)}
    };

    std::map<ShotType, cv::Point> last_point;

    for (size_t i = 0; i < data_series.size(); ++i) {
        double x_val = data_series[i].first;

        for (const auto& [shot, y_val] : data_series[i].second) {
            int x = margin + static_cast<int>((x_val - x_min) / (x_max - x_min) * (width - 2 * margin));
            int y = height - margin - static_cast<int>((y_val - y_min) / (y_max - y_min) * (height - 2 * margin));
            cv::Point pt(x, y);

            if (last_point.find(shot) != last_point.end()) {
                cv::line(plot, last_point[shot], pt, color_map[shot], 2);
            }

            last_point[shot] = pt;
        }
    }

    cv::putText(plot, title, cv::Point(margin, margin / 2), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,0,0), 2);
    cv::putText(plot, xlabel, cv::Point(width / 2, height - 10), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0,0,0), 1);
    cv::putText(plot, ylabel, cv::Point(10, height / 2), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0,0,0), 1);

    return plot;
}
