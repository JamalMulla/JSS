//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM3T_CELL_H
#define SIMULATOR_DRAM3T_CELL_H

#include <opencv2/core.hpp>

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Dram3tCell : public Memory {
    RTTR_ENABLE(Memory);

   private:
#ifdef TRACK_STATISTICS
    int cycle_count_ = 2;  // TODO calc proper numbers for cycle counts
    double refresh_time_ = 0.064;  // In S
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_width() override;
    double calc_height() override;
    double calc_dynamic_read() override;
    double calc_dynamic_write() override;
    cv::UMat scratch;
#endif

   public:
    Dram3tCell() = default;
    void init();

#ifdef TRACK_STATISTICS
    int get_cycle_count() override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_DRAM3T_CELL_H
