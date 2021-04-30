//
// Created by jm1417 on 09/04/2021.
//

#ifndef SIMULATOR_CLA_H
#define SIMULATOR_CLA_H

#include "simulator/base/component.h"
#include "simulator/base/config.h"
#include "simulator/registers/digital_register.h"
#include <opencv4/opencv2/core.hpp>

class CarryLookAheadAdder : public Component {
   private:
    int rows_;
    int cols_;
    int row_stride_;
    int col_stride_;
    int cycle_count_;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts for an addition
    double time_; // time in seconds for an addition
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
    int fun_transistor(int bits, const Config& config);
    double fun_static(int bits, const Config& config);
    double fun_dynamic(int bits, const Config& config);
    void fun_internal_mask(int rows, int cols, int row_stride, int col_stride);
    cv::Mat scratch;

   public:
    CarryLookAheadAdder(int rows, int cols, int row_stride, int col_stride, int bits, const Config& config);
#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    int get_cycle_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    cv::Mat get_transistor_count() override;
    void print_stats(const CycleCounter &counter) override;
    void inc_add();
#endif

    int add(int src1, int src2);
};

#endif  //SIMULATOR_CLA_H
