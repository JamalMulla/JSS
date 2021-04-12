//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_REGISTER_H
#define SIMULATOR_ANALOGUE_REGISTER_H

#include "register.h"

class AnalogueRegister : public Register {
   public:
    AnalogueRegister(int rows, int cols, int row_stride, int col_stride, Config& config, MemoryType memory = MemoryType::S2I);
    AnalogueRegister(int rows, int cols, int row_stride = 1, int col_stride = 1);
    AnalogueRegister(const cv::Mat& data, int row_stride = 1, int col_stride = 1);

    AnalogueRegister& operator()(const std::string& name);

#ifdef TRACK_STATISTICS
//    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_ANALOGUE_REGISTER_H
