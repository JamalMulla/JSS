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

class ProcessingElement {
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
    Config* config_;

    ProcessingElement(int rows, int columns, int row_stride, int col_stride, int num_analogue, int num_digital, Source source, const std::string &path, Config &config);
    void update_cycles(int cycles);


};

class ProcessingElement::builder {
   private:
    int rows_ = -1;
    int cols_ = -1;
    int row_stride_ = -1;
    int col_stride_ = -1;
    int num_analogue_ = -1;
    int num_digital_ = -1;
    Source source_;
    std::string path_;
    Config config_;

   public:
    builder& with_rows(int rows);
    builder& with_cols(int cols);
    builder& with_row_stride(int row_stride);
    builder& with_col_stride(int col_stride);
    builder& with_analogue_registers(int num_analogue);
    builder& with_digital_registers(int num_digital);
    builder& with_input_source(Source source);
    builder& with_file_path(const std::string& path);
    builder& with_config(Config& config);
    ProcessingElement build();
};

#endif  // SIMULATOR_PROCESSING_ELEMENT_H
