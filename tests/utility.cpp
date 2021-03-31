//
// Created by jm1417 on 31/03/2021.
//

#include "utility.h"

bool utility::are_mats_equal(const cv::Mat& mat1, const cv::Mat& mat2) {
    if (mat1.dims == mat2.dims &&
        mat1.size == mat2.size &&
        mat1.elemSize() == mat2.elemSize()) {
        if (mat1.isContinuous() && mat2.isContinuous()) {
            return 0 == memcmp(mat1.ptr(), mat2.ptr(), mat1.total() * mat1.elemSize());
        } else {
            return cv::countNonZero(mat1!=mat2) == 0;
        }
    }
    return false;
}
