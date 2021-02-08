//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H

#include <vector>
#include "photodiode.h"
#include "../registers/digital_register.h"
#include "../units/Squarer.h"
#include "../units/comparator.h"
#include "../buses/analogue_bus.h"
#include "../buses/digital_bus.h"


class ProcessingElement : public Component {
public:
    Photodiode photodiode;
    std::vector<AnalogueRegister> analogue_registers;
    std::vector<DigitalRegister> digital_registers;
    Squarer squarer;
    Comparator comparator;
    AnalogueBus analogue_bus;
    DigitalBus local_read_bus;
    DigitalBus local_write_bus;

    ProcessingElement(int rows, int columns, int num_analogue_regs, int num_digital_regs);
    void print_stats() override;

    };


#endif //SIMULATOR_PROCESSING_ELEMENT_H
