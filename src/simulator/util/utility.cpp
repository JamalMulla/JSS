//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/util/utility.h"

#include <opencv4/opencv2/highgui.hpp>

void utility::remap_register(Register &reg, cv::Mat &dst) {
    /* reg.read().convertTo(dst, CV_8U, 255.0 / (reg.max_val - reg.min_val), */
    /*                       -reg.min_val * 255.0 / (reg.max_val - reg.min_val)); */

    double min, max;
    cv::minMaxLoc(reg.read(), &min, &max);
    std::cout << "REG: " << reg.name_ << "MIN: " << min << "MAX: " << max << std::endl;

    reg.read().convertTo(dst, CV_8U, 255, 0);
    /* dst.convertTo(dst, CV_8U, 255.0 / (reg.max_val - reg.min_val), */

//    double minVal, maxVal;
//    cv::minMaxLoc(reg.value(), &minVal, &maxVal);
//    if(maxVal == minVal) {
//        minVal = 0;
//    }
//    reg.value().convertTo(dst, CV_8U, 255.0 / (maxVal - minVal),
//                  -minVal * 255.0 / (maxVal - minVal));
}

void utility::remap_mat(cv::Mat &src, cv::Mat &dst) {
    double minVal, maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    if(maxVal == minVal) {
        minVal = 0;
    }
    src.convertTo(dst, CV_8U, 255.0 / (maxVal - minVal),
                  -minVal * 255.0 / (maxVal - minVal));
}

int utility::normalise(int value, int old_low, int old_high, int new_low,
                       int new_high) {
    return (new_high - new_low) * ((value - old_low) / (old_high - old_low)) +
           new_low;
}

std::string utility::opencv_type_to_str(int type) {
    // Taken from https://stackoverflow.com/a/17820615
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch(depth) {
        case CV_8U: r = "8U"; break;
        case CV_8S: r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default: r = "User"; break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

void utility::NOT_IMPLEMENTED() {
    std::cerr << "This function has not been implemented" << std::endl;
    exit(EXIT_FAILURE);
}

//    int e1 = cv::getTickCount();
//    int e2 = cv::getTickCount();
//    double time = (e2 - e1)/cv::getTickFrequency();
//    std::cout << "Took " << time << " seconds for scamp5_load_pattern" <<
//    std::endl;
