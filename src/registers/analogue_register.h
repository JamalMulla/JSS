//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_REGISTER_H
#define SIMULATOR_ANALOGUE_REGISTER_H

#include "register.h"

class AnalogueRegister : public Register {
public:
    AnalogueRegister(int rows, int columns);

    Data read() override;
    void write(Data data) override;
    void write(int data) override;
    void print_stats() override;
};


#endif //SIMULATOR_ANALOGUE_REGISTER_H
