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
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_dynamic_read() override;
    double calc_dynamic_write() override;
    double calc_width() override;
    double calc_height() override;
    cv::UMat scratch;
#endif

   public:
    Sram6tCell() = default;
    void init();

#ifdef TRACK_STATISTICS
    int get_cycle_count() override;
#endif
};

#endif  // SIMULATOR_SRAM6T_CELL_H
