//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H

#include <iostream>
#include <vector>

#include "simulator/base/pixel.h"
#include "simulator/buses/analogue_bus.h"
#include "simulator/buses/digital_bus.h"
#include "simulator/registers/digital_register.h"
#include "simulator/units/comparator.h"
#include "simulator/units/squarer.h"

class ProcessingElement : public Component {
   public:
    class builder;
    Pixel photodiode;
    std::vector<AnalogueRegister> analogue_registers;
    std::vector<DigitalRegister> digital_registers;
    Squarer squarer;
    Comparator comparator;
    AnalogueBus analogue_bus;
    DigitalBus local_read_bus;
    DigitalBus local_write_bus;

    ProcessingElement(int rows, int columns, int num_analogue, int num_digital,
                      Source source, const std::string& path);
#ifdef TRACK_STATISTICS
    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

class ProcessingElement::builder {
   private:
    int rows_ = -1;
    int cols_ = -1;
    int num_analogue_ = -1;
    int num_digital_ = -1;
    Source source_;
    std::string path_;

   public:
    builder& with_rows(int rows);
    builder& with_cols(int cols);
    builder& with_analogue_registers(int num_analogue);
    builder& with_digital_registers(int num_digital);
    builder& with_input_source(Source source);
    builder& with_file_path(const std::string& path);
    ProcessingElement build();
};

#endif  // SIMULATOR_PROCESSING_ELEMENT_H
