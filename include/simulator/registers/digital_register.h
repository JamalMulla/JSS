//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>

#include "register.h"
#include "simulator/memory/dram_3t_cell.h"

class DigitalRegister : public Register {
   public:
    DigitalRegister(int rows, int columns, Memory& memory_type);
    DigitalRegister(int rows, int columns);
    DigitalRegister(const cv::Mat& data, Memory& memory_type);
    DigitalRegister(const cv::Mat& data);

    DigitalRegister& operator()(const std::string& name);
    void set();
    void clear();

#ifdef TRACK_STATISTICS
//    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_DIGITAL_REGISTER_H
