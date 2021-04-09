//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM_H
#define SIMULATOR_DRAM_H

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class DRAM : public Memory {
   protected:
    double write_access_time_;  // In s
    double read_access_time_;  // In s
    double write_power_draw_;  // In W
    double read_power_draw_;  // In W
    double retention_time_;  // In S
    Config* config_;

   public:
    DRAM(Config& config);
    double get_static_power(double time) override;
    double get_dynamic_power() override;
    int get_cycle_count() override;
    int get_transistor_count() override;
};

#endif  // SIMULATOR_DRAM_H
