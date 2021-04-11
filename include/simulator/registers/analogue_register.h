//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_REGISTER_H
#define SIMULATOR_ANALOGUE_REGISTER_H

#include "register.h"

class AnalogueRegister : public Register {
   public:
    AnalogueRegister(int rows, int columns, Memory& memory);
    AnalogueRegister(int rows, int columns);
    AnalogueRegister(const cv::Mat& data, Memory& memory);
    AnalogueRegister(const cv::Mat& data);

    AnalogueRegister& operator()(const std::string& name);

#ifdef TRACK_STATISTICS
//    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_ANALOGUE_REGISTER_H
