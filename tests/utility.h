//
// Created by jm1417 on 31/03/2021.
//

#ifndef SIMULATOR_TESTS_UTILITY_H
#define SIMULATOR_TESTS_UTILITY_H

#include <iostream>
#include <opencv2/core.hpp>

namespace utility {
bool mats_are_equal(const cv::Mat& mat1, const cv::Mat& mat2);

template<typename interpret_type>
void print_matrix(const cv::Mat& mat);

template<typename interpret_type>
void print_matrix(const cv::Mat& mat) {
    for(int r = 0; r < mat.rows; r++) {
        for(int c = 0; c < mat.cols; c++) {
            std::cout << (int)mat.at<interpret_type>(r, c) << ", ";
        }
        std::cout << std::endl;
    }
}

}  // namespace utility

#endif  // SIMULATOR_TESTS_UTILITY_H
