//
// Created by jm1417 on 29/05/2021.
//

#include "simulator/base/opencv_wrappers.h"
#include <opencv2/cudaarithm.hpp>

void ocv_wrappers::arith::add(cv::OutputArray dst, cv::InputArray src1, cv::InputArray src2) {
#ifdef USE_CUDA
    cv::cuda::add(src1, src2, dst);
#else
    cv::add(src1, src2, dst);
#endif
}

void ocv_wrappers::arith::add(cv::OutputArray dst, cv::InputArray src1, cv::InputArray src2, cv::InputArray mask) {
#ifdef USE_CUDA
    cv::cuda::add(src1, src2, dst, mask);
#else
    cv::add(src1, src2, dst, mask);
#endif
}
