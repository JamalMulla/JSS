//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <opencv2/core.hpp>
#include "simulator/base/component.h"

class Memory : public Component {
   protected:
    int rows_;
    int cols_;
    int row_stride_;
    int col_stride_;
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components

    void fun_internal_mask(int rows, int cols, int row_stride, int col_stride);
   public:
    Memory(int rows, int cols, int row_stride, int col_stride);
    virtual cv::Mat& get_transistor_count() override = 0;
    virtual int get_cycle_count() override = 0;
    virtual cv::Mat& get_dynamic_power() override = 0;
    virtual cv::Mat& get_static_power() override = 0;
    virtual void update(double time) override = 0;
    virtual void read(const cv::_InputOutputArray &mask) = 0;
    virtual void read() = 0;
    virtual void write(const cv::_InputOutputArray &mask) = 0;
    virtual void write() = 0;
};

#endif  // SIMULATOR_MEMORY_H
