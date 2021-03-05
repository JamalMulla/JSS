//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/sram.h"

SRAM::SRAM(float write_power, float read_power)
    : MemoryType(0, 0, write_power, read_power) {}

