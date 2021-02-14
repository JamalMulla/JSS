//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_SRAM_H
#define SIMULATOR_SRAM_H


#include "../memory_type.h"

class SRAM : public MemoryType {
public:
    SRAM(float write_power, float read_power);
};


#endif //SIMULATOR_SRAM_H
