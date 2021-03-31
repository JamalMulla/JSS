//
// Created by jm1417 on 31/03/2021.
//

#include <iostream>
#include "utility.h"

bool utility::are_mats_equal(const cv::Mat& mat1, const cv::Mat& mat2) {
    if (mat1.dims == mat2.dims &&
        mat1.size == mat2.size &&
        mat1.elemSize() == mat2.elemSize()) {
        if (mat1.isContinuous() && mat2.isContinuous()) {
            return 0 == memcmp(mat1.ptr(), mat2.ptr(), mat1.total() * mat1.elemSize());
        } else {
            const cv::Mat *arrays[] = {&mat1, &mat2, nullptr};
            uchar *ptrs[2];
            cv::NAryMatIterator it(arrays, ptrs, 2);
            for (unsigned int p = 0; p < it.nplanes; p++, ++it)
                if (0 != memcmp(it.ptrs[0], it.ptrs[1], it.size * mat1.elemSize()))
                    return false;

            return true;
        }
    }
    return false;
}

void utility::print_matrix(const cv::Mat &mat, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            std::cout << mat.at<int>(r, c) << ", ";
        }
        std::cout << std::endl;
    }
}
