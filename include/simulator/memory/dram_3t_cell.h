//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM_3T_CELL_H
#define SIMULATOR_DRAM_3T_CELL_H

#include "simulator/base/config.h"
#include "simulator/memory/memory.h"

class Dram3tCell : public Memory {
   private:
    int cycle_count_ = 2; // TODO find proper numbers for cycle counts
    int transistor_count_ = 3;
    double static_power_;  // in Watts
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    double refresh_time_ = 0.064;  // In S
    double dynamic_power_ = 0; // Changed when dram cell is read from/written to
    Config* config_;
    double fun_static(const Config& config);
    double fun_dynamic_read(const Config& config);
    double fun_dynamic_write(const Config& config);

   public:
    Dram3tCell() = default;
    explicit Dram3tCell(Config& config);
    double get_static_power(double time) override;
    double get_dynamic_power() override;
    int get_cycle_count() override;
    int get_transistor_count() override;
    void read();
    void write();
};

#endif  // SIMULATOR_DRAM_3T_CELL_H
