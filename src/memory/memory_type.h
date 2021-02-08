//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_TYPE_H
#define SIMULATOR_MEMORY_TYPE_H


class MemoryType {
protected:
    float write_access_time; // In ns
    float read_access_time; // In ns
    float write_power_draw; // In μW
    float read_power_draw; // In μW

public:
    MemoryType(float write_access_time, float read_access_time, float write_power_draw, float read_power_draw);

};


#endif //SIMULATOR_MEMORY_TYPE_H
