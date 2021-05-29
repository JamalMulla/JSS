//
// Created by jm1417 on 29/05/2021.
//

#ifndef SIMULATOR_OPENCV_WRAPPERS_H
#define SIMULATOR_OPENCV_WRAPPERS_H

#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>

/*Wrappers around common OpenCV functionality which makes GPU switching easy */

namespace ocv_wrappers::arith {
    void add(cv::OutputArray dst, cv::InputArray src1, cv::InputArray src2);
    void add(cv::OutputArray dst, cv::InputArray src1, cv::InputArray src2, cv::InputArray mask);

}



#endif  //SIMULATOR_OPENCV_WRAPPERS_H
