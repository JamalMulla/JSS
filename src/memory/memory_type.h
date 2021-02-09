//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_TYPE_H
#define SIMULATOR_MEMORY_TYPE_H


class MemoryType {
public:
    double write_access_time; // In ns
    double read_access_time; // In ns
    double write_power_draw; // In μW
    double read_power_draw; // In μW

public:
    MemoryType(float write_access_time, float read_access_time, float write_power_draw, float read_power_draw);

    MemoryType();

};


#endif //SIMULATOR_MEMORY_TYPE_H
