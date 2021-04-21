//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv4/opencv2/core/mat.hpp>

#include "register.h"
#include "simulator/memory/dram3t_cell.h"

class DigitalRegister : public Register {
   public:
    DigitalRegister(int rows, int columns, Config& config, int row_stride = 1, int col_stride = 1, MemoryType memory_type = MemoryType::DRAM3T);
    DigitalRegister(int rows, int cols, int row_stride = 1, int col_stride = 1);
    DigitalRegister(const cv::Mat& data, int row_stride = 1, int col_stride = 1);

    DigitalRegister& operator()(const std::string& name);
    void set();
    void clear();

#ifdef TRACK_STATISTICS
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_DIGITAL_REGISTER_H
