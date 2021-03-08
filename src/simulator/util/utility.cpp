//
// Created by jm1417 on 08/02/2021.
//

#include <opencv2/highgui.hpp>
#include "simulator/util/utility.h"

void utility::remap_register(Register &reg, cv::Mat &dst) {
    double minVal, maxVal;
    cv::minMaxLoc(reg.value(), &minVal, &maxVal);
    if (maxVal == minVal) {
        minVal = 0;
    }
    reg.value().convertTo(dst, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
}

void utility::display_register(const std::string &window_name, Register &reg) {
    cv::Mat draw;
    remap_register(reg, draw);
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, 256, 256);
    cv::setMouseCallback(window_name, utility::on_mouse_reg, &reg);
    cv::imshow(window_name, draw);
}

// TODO reduce duplication
void utility::remap_mat(cv::Mat &src, cv::Mat &dst) {
    double minVal, maxVal;
    cv::minMaxLoc(src, &minVal, &maxVal);
    if (maxVal == minVal) {
        minVal = 0;
    }
    src.convertTo(dst, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
}

void utility::display_mat(const std::string &window_name, cv::Mat &src) {
    cv::Mat draw;
    remap_mat(src, draw);
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, 256, 256);
    cv::setMouseCallback(window_name, utility::on_mouse_mat, &src);
    cv::imshow(window_name, draw);
}

void utility::on_mouse_mat(int event, int x, int y, int, void *src) {
    if ( event != cv::EVENT_MOUSEMOVE ) return;
    auto* dr = static_cast<cv::Mat*>(src);
    if (y < 0 || x < 0 || y > dr->rows || x > dr->cols) return;
    std::cout<<"("<<x<<", "<<y<<") ......  "<< dr->at<int16_t>(y,x) <<'\n';
}

void utility::on_mouse_reg(int event, int x, int y, int, void* reg) {
    if ( event != cv::EVENT_MOUSEMOVE ) return;
    auto* dr = static_cast<Register*>(reg);
    if (y < 0 || x < 0 || y > dr->value().rows || x > dr->value().cols) return;
    std::cout<<"("<<x<<", "<<y<<") ......  "<< dr->value().at<int16_t>(y,x) <<'\n';
}

int utility::normalise(int value, int old_low, int old_high, int new_low, int new_high) {
    return (new_high - new_low) * ((value - old_low)/(old_high - old_low)) + new_low;
}

std::string utility::opencv_type_to_str(int type) {
    // Taken from https://stackoverflow.com/a/17820615
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch ( depth ) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }

    r += "C";
    r += (chans+'0');

    return r;
}


//    int e1 = cv::getTickCount();
//    int e2 = cv::getTickCount();
//    double time = (e2 - e1)/cv::getTickFrequency();
//    std::cout << "Took " << time << " seconds for scamp5_load_pattern" << std::endl;
