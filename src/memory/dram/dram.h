//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_DRAM_H
#define SIMULATOR_DRAM_H


#include "../memory_type.h"

class DRAM : public MemoryType {
protected:
    float retention_time; // In ns
public:
    DRAM(float writeAccessTime, float readAccessTime, float writePowerDraw, float readPowerDraw, float retentionTime);
};


#endif //SIMULATOR_DRAM_H
