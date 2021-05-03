//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SRAM6T_CELL_H
#define SIMULATOR_SRAM6T_CELL_H

#include <opencv4/opencv2/core.hpp>

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Sram6tCell : public Memory {
   private:
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    int transistor_count_ = 6;
    double static_power_;  // in Watts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    std::shared_ptr<Config> config_;
    double time_; // Time in seconds it takes for read/write
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
    double fun_static(const std::shared_ptr<Config>& config);
    double fun_dynamic_read(const std::shared_ptr<Config>& config);
    double fun_dynamic_write(const std::shared_ptr<Config>& config);
    cv::Mat scratch;
#endif

   public:
    explicit Sram6tCell(int rows, int cols, int row_stride, int col_stride, const std::shared_ptr<Config>& config);

#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    int get_cycle_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    cv::Mat get_transistor_count() override;
    void read(const cv::_InputOutputArray& mask) override;
    void read() override;
    void write(const cv::_InputOutputArray& mask) override;
    void write() override;
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_SRAM6T_CELL_H
