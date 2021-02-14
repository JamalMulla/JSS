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
    class builder;
    Photodiode photodiode;
    std::vector<AnalogueRegister> analogue_registers;
    std::vector<DigitalRegister> digital_registers;
    Squarer squarer;
    Comparator comparator;
    AnalogueBus analogue_bus;
    DigitalBus local_read_bus;
    DigitalBus local_write_bus;

    ProcessingElement(int rows, int columns, std::vector<AnalogueRegister>  analogue_regs, std::vector<DigitalRegister>  digital_regs);

    void print_stats(CycleCounter counter) override;

};

class ProcessingElement::builder {
private:
    int rows_ = -1;
    int cols_ = -1;
    std::vector<AnalogueRegister> analogue_regs_ = {};
    std::vector<DigitalRegister> digital_regs_ = {};
public:
    builder& with_rows(int rows);
    builder& with_cols(int cols);
    builder& with_digital_registers(const std::vector<MemoryType>& types);
    builder& with_analogue_registers(const std::vector<MemoryType>& types);
    ProcessingElement build();
};


#endif //SIMULATOR_PROCESSING_ELEMENT_H
