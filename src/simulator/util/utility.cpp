//
// Created by jm1417 on 08/02/2021.
//

#include <opencv2/highgui.hpp>
#include <simulator/registers/digital_register.h>
#include "simulator/util/utility.h"

Register& utility::remap_image(Register &reg) {
    // TODO fix issues when there's only one colour. Range isn't correct
    double minVal, maxVal;
    cv::minMaxLoc(reg.value(), &minVal, &maxVal);
    reg.value().convertTo(reg.value(), CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    return reg;
}

void utility::display_register(const std::string &window_name, Register &reg) {
    remap_image(reg);
    cv::namedWindow(window_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(window_name, 256, 256);
    cv::setMouseCallback(window_name, utility::onMouse, &reg);
    cv::imshow(window_name, reg.value());
}

void utility::onMouse(int event, int x, int y, int, void* reg) {
    if ( event != cv::EVENT_MOUSEMOVE ) return;
    auto* dr = static_cast<DigitalRegister*>(reg);
    std::cout<<"("<<x<<", "<<y<<") ......  "<< (int) dr->value().at<uint8_t>(y,x) <<'\n';
}

int utility::normalise(int value, int old_low, int old_high, int new_low, int new_high) {
    return (new_high - new_low) * ((value - old_low)/(old_high - old_low)) + new_low;
}


//    int e1 = cv::getTickCount();
//    int e2 = cv::getTickCount();
//    double time = (e2 - e1)/cv::getTickFrequency();
//    std::cout << "Took " << time << " seconds for scamp5_load_pattern" << std::endl;
