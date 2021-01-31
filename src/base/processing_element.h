//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H


#include "photodiode.h"
#include "../registers/analogue_register.h"
#include "../registers/digital_register.h"
#include "../buses/analogue_bus.h"
#include "../buses/digital_bus.h"
#include "../units/Squarer.h"
#include "../units/comparator.h"
#include <vector>
#include <iostream>

class ProcessingElement {
public:
    Photodiode photodiode;
    std::vector<AnalogueRegister> analogue_registers;
    std::vector<DigitalRegister> digital_registers;
    Squarer squarer;
    Comparator comparator;
    AnalogueBus analogue_bus;
    DigitalBus local_read_bus;
    DigitalBus local_write_bus;
    AnalogueRegister news;
    DigitalRegister flag;
    ProcessingElement(int num_analogue_regs, int num_digital_regs) {
        for (int i = 0; i < num_analogue_regs; i++) {
            analogue_registers.emplace_back(AnalogueRegister());
        }
        for (int i = 0; i < num_digital_regs; i++) {
            digital_registers.emplace_back(DigitalRegister());
        }
    }
};


#endif //SIMULATOR_PROCESSING_ELEMENT_H
