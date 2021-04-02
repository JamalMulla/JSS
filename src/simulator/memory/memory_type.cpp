//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/memory_type.h"

MemoryType::MemoryType(float write_access_time, float read_access_time,
                       float write_power_draw, float read_power_draw)
    : write_access_time(write_access_time),
      read_access_time(read_access_time),
      write_power_draw(write_power_draw),
      read_power_draw(read_power_draw) {}

MemoryType::MemoryType()
    : write_access_time(-1),
      read_access_time(-1),
      write_power_draw(-1),
      read_power_draw(-1) {}
