//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM_H
#define SIMULATOR_DRAM_H


#include "simulator/memory/memory_type.h"

class DRAM : public MemoryType {
protected:
    float retention_time_; // In S
public:
    DRAM(float write_access_time, float read_access_time, float write_power_draw, float read_power_draw, float retention_time);
};


#endif //SIMULATOR_DRAM_H
