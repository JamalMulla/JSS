//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM_3T_CELL_H
#define SIMULATOR_DRAM_3T_CELL_H

#include <opencv2/core.hpp>

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Dram3tCell : public Memory {
   private:
    int rows_;
    int cols_;
    int row_stride_;
    int col_stride_;
    int cycle_count_ = 2;  // TODO find proper numbers for cycle counts
    int transistor_count_ = 3;
    double static_power_;  // in Watts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    Config* config_;
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
    cv::Mat array_transistor_count_;
    cv::Mat array_static_power_;
    cv::Mat array_dynamic_power_;
    double refresh_time_ = 0.064;  // In S
    double fun_static(const Config& config);
    double fun_dynamic_read(const Config& config);
    double fun_dynamic_write(const Config& config);
    void fun_internal_mask(int rows, int cols, int row_stride, int col_stride);
    cv::Mat scratch;

public:
    explicit Dram3tCell(int rows, int cols, int row_stride, int col_stride, Config& config);

    void update(double time) override;
    int get_cycle_count() override;
    cv::Mat& get_static_power() override;
    cv::Mat& get_dynamic_power() override;
    cv::Mat& get_transistor_count() override;
    void read(const cv::_InputOutputArray& mask) override;
    void read() override;
    void write(const cv::_InputOutputArray& mask) override;
    void write() override;
};

#endif  // SIMULATOR_DRAM_3T_CELL_H
