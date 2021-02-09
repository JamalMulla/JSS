//
// Created by jm1417 on 08/02/2021.
//

#include "dram.h"

DRAM::DRAM(float writeAccessTime, float readAccessTime, float writePowerDraw, float readPowerDraw, float retentionTime)
        :MemoryType(writeAccessTime, readAccessTime, writePowerDraw, readPowerDraw),
        retention_time(retentionTime) {}
