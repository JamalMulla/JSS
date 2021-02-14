//
// Created by jm1417 on 08/02/2021.
//

#include "dram.h"

DRAM::DRAM(float write_access_time, float read_access_time, float write_power_draw, float read_power_draw, float retention_time)
        : MemoryType(write_access_time, read_access_time, write_power_draw, read_power_draw),
          retention_time_(retention_time) {}
