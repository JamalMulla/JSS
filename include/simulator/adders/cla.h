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
    RTTR_ENABLE(Component);
   private:
    int bits_;
    int cycle_count_;
    double time_; // time in seconds for an addition
#ifdef TRACK_STATISTICS
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_width() override;
    double calc_height() override;
#endif
    cv::Mat scratch;

   public:
    CarryLookAheadAdder() = default;
    void init();

    void set_bits(int bits);

    int add(int src1, int src2);
#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter &counter) override;
    void inc_add();
#endif
};

#endif  //SIMULATOR_CLA_H
