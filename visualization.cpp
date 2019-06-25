#include "visualization.h"


void visualize(const std::vector<std::vector<double>> &field, std::string &filename, const double &min, const double &max){
    cv::Mat vis_field((int) field.size(), (int) field[0].size(), CV_8UC3, cv::Scalar(255, 255, 255));
    cv::Vec3b color;
    double ratio;

    int i = 0, j;
    for (auto &row: field){
        j = 0;
        for (auto &el: row){
            ratio = (el - min) / (max - min);
            color[1] = (uchar) std::max(0.0, 255 * (ratio - 1));
            color[0] = (uchar) std::max(0.0, 255 * (1 - ratio));
            color[2] = (uchar) (255 - color[0] - color[1]);
            vis_field.at<cv::Vec3b>(cv::Point(j, i)) = color;
            j++;
        }
        i++;
    }
//
//    for (auto &row: field){
//        for (auto &el: row){
//            std::cout << el << " ";
//        }
//        std::cout << std::endl;
//    }

    cv::imwrite(filename, vis_field);
}

