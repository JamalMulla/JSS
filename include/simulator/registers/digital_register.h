//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>

#include "register.h"
#include "simulator/memory/dram_3t.h"

class DigitalRegister : public Register {
   public:
    DigitalRegister(int rows, int columns,
                    const MemoryType& memory_type = DRAM_3T());
    DigitalRegister(const Data& data,
                    const MemoryType& memory_type = DRAM_3T());

    DigitalRegister& operator()(const std::string& name);
    void set();
    void clear();

    Data read() override;
    // TODO here and in AnalogueRegister make reference
    void write(Data data) override;
    void write(int data) override;

#ifdef TRACK_STATISTICS
    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_DIGITAL_REGISTER_H
