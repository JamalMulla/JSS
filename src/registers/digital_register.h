//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "register.h"
#include "../memory/dram/dram_3t.h"

class DigitalRegister : public Register {
public:
    DigitalRegister(int rows, int columns, const MemoryType& memory_type = DRAM_3T());
    DigitalRegister& operator()(const std::string& name);
    void set();
    void clear();

    Data read() override;
    void write(Data data) override;
    void write(int data) override;
    void print_stats(CycleCounter counter) override;
};


#endif //SIMULATOR_DIGITAL_REGISTER_H
