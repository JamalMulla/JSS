//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <opencv2/core.hpp>

class Memory : public Component {
   public:
    cv::Mat& get_transistor_count() override = 0;
    int get_cycle_count() override = 0;
    cv::Mat& get_dynamic_power() override = 0;
    cv::Mat& get_static_power() override = 0;
    void update(double time) override = 0;
    virtual void read(const cv::_InputOutputArray &mask) = 0;
    virtual void read() = 0;
    virtual void write(const cv::_InputOutputArray &mask) = 0;
    virtual void write() = 0;
};

#endif  // SIMULATOR_MEMORY_H
