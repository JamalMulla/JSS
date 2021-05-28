//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SRAM6T_CELL_H
#define SIMULATOR_SRAM6T_CELL_H

#include <opencv2/core.hpp>

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Sram6tCell : public Memory {
    RTTR_ENABLE(Memory);
   private:
    int read_count_ = 0;
    int write_count_ = 0;
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    double time_; // Time in seconds it takes for read/write
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_dynamic_read();
    double calc_dynamic_write();
    double calc_width() override;
    double calc_height() override;
    cv::Mat scratch;
#endif

   public:
    Sram6tCell() = default;
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

#endif  // SIMULATOR_SRAM6T_CELL_H
