//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_UTILITY_H
#define SIMULATOR_UTILITY_H

#include <string>
#include <opencv2/opencv.hpp>
#include "simulator/registers/register.h"

namespace utility {

    // Vision
    template <typename interpret_type>
    void display_register(const std::string &window_name, Register &reg);
    template <typename interpret_type>
    void on_mouse_reg(int event, int x, int y, int, void* reg);
    template <typename interpret_type>
    void display_mat(const std::string& window_name, cv::Mat& src);
    template <typename interpret_type>
    void on_mouse_mat(int event, int x, int y, int, void* src);
    void remap_register(Register& reg, cv::Mat& dst);
    void remap_mat(cv::Mat& src, cv::Mat& dst);
    int normalise(int value, int old_low, int old_high, int new_low, int new_high);


    // Type
    std::string opencv_type_to_str(int type);

    template <typename interpret_type>
    void display_register(const std::string &window_name, Register &reg) {
        cv::Mat draw;
        remap_register(reg, draw);
        cv::namedWindow(window_name, cv::WINDOW_NORMAL);
        cv::resizeWindow(window_name, 256, 256);
        cv::setMouseCallback(window_name, utility::on_mouse_reg<interpret_type>, &reg);
        cv::imshow(window_name, draw);
    }

    template <typename interpret_type>
    void on_mouse_reg(int event, int x, int y, int, void* reg) {
        if ( event != cv::EVENT_MOUSEMOVE ) return;
        auto* dr = static_cast<Register*>(reg);
        if (y < 0 || x < 0 || y > dr->value().rows || x > dr->value().cols) return;
        std::cout<<"("<<x<<", "<<y<<") ......  "<< (int) dr->value().at<interpret_type>(y,x) <<'\n';
    }

    template <typename interpret_type>
    void display_mat(const std::string &window_name, cv::Mat &src) {
        cv::Mat draw;
        remap_mat(src, draw);
        cv::namedWindow(window_name, cv::WINDOW_NORMAL);
        cv::resizeWindow(window_name, 256, 256);
        cv::setMouseCallback(window_name, utility::on_mouse_mat<interpret_type>, &src);
        cv::imshow(window_name, draw);
    }

    template <typename interpret_type>
    void on_mouse_mat(int event, int x, int y, int, void *src) {
        if ( event != cv::EVENT_MOUSEMOVE ) return;
        auto* dr = static_cast<cv::Mat*>(src);
        if (y < 0 || x < 0 || y > dr->rows || x > dr->cols) return;
        std::cout<<"("<<x<<", "<<y<<") ......  "<< (int) dr->at<interpret_type>(y,x) <<'\n';
    }

}


#endif //SIMULATOR_UTILITY_H
