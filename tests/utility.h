//
// Created by jm1417 on 31/03/2021.
//

#ifndef SIMULATOR_UTILITY_H
#define SIMULATOR_UTILITY_H


#include <opencv2/core.hpp>

namespace utility {
    bool are_mats_equal(const cv::Mat& mat1, const cv::Mat& mat2);
    void print_matrix(const cv::Mat& mat, int rows, int cols);
}


#endif //SIMULATOR_UTILITY_H
