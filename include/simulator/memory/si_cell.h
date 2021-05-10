//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SI_CELL_H
#define SIMULATOR_SI_CELL_H

#include "simulator/memory/memory.h"

class SiCell : public Memory {
   private:
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    int transistor_count_ = 7;
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    double time_; // time in seconds for a read/write
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_dynamic_read();
    double calc_dynamic_write();
    cv::Mat scratch;
#endif

   public:
    SiCell() = default;
    void init();

#ifdef TRACK_STATISTICS
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

#endif  // SIMULATOR_SI_CELL_H
