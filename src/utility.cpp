//
// Created by jm1417 on 08/02/2021.
//

#include <opencv2/highgui.hpp>
#include "utility.h"

void utility::display_register(const std::string &window_name, Register &reg) {
    double minVal, maxVal;
    cv::minMaxLoc(reg.value(), &minVal, &maxVal);
    cv::Mat draw;
    reg.value().convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    //cv::resizeWindow(window_name, 256, 256);
    cv::imshow(window_name, draw);
}
