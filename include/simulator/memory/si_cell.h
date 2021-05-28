//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SI_CELL_H
#define SIMULATOR_SI_CELL_H

#include "simulator/memory/memory.h"

class SiCell : public Memory {
    RTTR_ENABLE(Memory);
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;  // TODO find proper numbers for cycle counts
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_dynamic_read() override;
    double calc_dynamic_write() override;
    double calc_width() override;
    double calc_height() override;
#endif

   public:
    SiCell() = default;
    void init();

};

#endif  // SIMULATOR_SI_CELL_H
