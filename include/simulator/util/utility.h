//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_UTILITY_H
#define SIMULATOR_UTILITY_H

#include <string>
#include "simulator/registers/register.h"

namespace utility {

    // Vision
    void remap_register(Register& reg, cv::Mat& dst);
    void display_register(const std::string& window_name, Register& reg);
    void remap_mat(cv::Mat& src, cv::Mat& dst);
    void display_mat(const std::string& window_name, cv::Mat& src);
    void on_mouse_reg(int event, int x, int y, int, void* reg);
    void on_mouse_mat(int event, int x, int y, int, void* src);
    int normalise(int value, int old_low, int old_high, int new_low, int new_high);


    // Type
    std::string opencv_type_to_str(int type);
}


#endif //SIMULATOR_UTILITY_H
