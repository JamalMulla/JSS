//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM3T_CELL_H
#define SIMULATOR_DRAM3T_CELL_H

#include <opencv4/opencv2/core.hpp>

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Dram3tCell : public Memory {
    RTTR_ENABLE(Memory);
   private:
    int read_count_ = 0;
    int write_count_ = 0;
#ifdef TRACK_STATISTICS
    int cycle_count_ = 2;  // TODO find proper numbers for cycle counts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    double time_; // time in seconds for read/write
    double refresh_time_ = 0.064;  // In S
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_width() override;
    double calc_height() override;
    double calc_dynamic_read();
    double calc_dynamic_write();
    cv::Mat scratch;
#endif

public:

    Dram3tCell() = default;
    void init();

#ifdef TRACK_STATISTICS
    cv::Mat get_dynamic_energy_array() override;
    void update_static(double time) override;
    int get_cycle_count() override;
    void read(const cv::_InputOutputArray& mask) override;
    void read() override;
    void write(const cv::_InputOutputArray& mask) override;
    void write() override;
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_DRAM3T_CELL_H
