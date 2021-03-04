//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H

#include <vector>
#include "simulator/base/photodiode.h"
#include "simulator/registers/digital_register.h"
#include "simulator/units/squarer.h"
#include "simulator/units/comparator.h"
#include "simulator/buses/analogue_bus.h"
#include "simulator/buses/digital_bus.h"

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

    ProcessingElement(int rows, int columns, int num_analogue, int num_digital);

    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;

};

class ProcessingElement::builder {
private:
    int rows_ = -1;
    int cols_ = -1;
    int num_analogue_ = -1;
    int num_digital_ = -1;
public:
    builder& with_rows(int rows);
    builder& with_cols(int cols);
    builder& with_analogue_registers(int num_analogue);
    builder& with_digital_registers(int num_digital);
    ProcessingElement build();
};


#endif //SIMULATOR_PROCESSING_ELEMENT_H
