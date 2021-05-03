//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PROCESSING_ELEMENT_H
#define SIMULATOR_PROCESSING_ELEMENT_H

#include <simulator/external/parser.h>

#include <iostream>
#include <vector>

#include "simulator/base/pixel.h"
#include "simulator/buses/analogue_bus.h"
#include "simulator/buses/digital_bus.h"
#include "simulator/registers/digital_register.h"
#include "simulator/units/comparator.h"
#include "simulator/units/squarer.h"

class ProcessingElement : public Component {
   protected:
    int rows_;
    int cols_;
    int row_stride_ = 1;
    int col_stride_ = 1;
    std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers_;
    std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers_;
    std::shared_ptr<Pixel> pixel_;
    std::shared_ptr<Config> config_;

   public:
    Squarer squarer;
    Comparator comparator;
    AnalogueBus analogue_bus;
    DigitalBus local_read_bus;
    [[maybe_unused]] DigitalBus local_write_bus;

    ProcessingElement() = default;

    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_analogue_registers(std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers);
    void set_digital_registers(std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers);
    void set_pixel(std::shared_ptr<Pixel> pixel);
    void set_config(std::shared_ptr<Config> config);

    rttr::variant analogue_registers_converter(json& j, std::unordered_map<std::string, rttr::variant>& cache);
    rttr::variant digital_registers_converter(json& j, std::unordered_map<std::string, rttr::variant>& cache);
    rttr::variant pixel_converter(json& j);

    void add_analogue_register(const std::string& name, std::shared_ptr<AnalogueRegister> reg);
    void add_digital_register(const std::string& name, std::shared_ptr<DigitalRegister> reg);
    std::shared_ptr<AnalogueRegister> get_analogue_register(const std::string& name);
    std::shared_ptr<DigitalRegister> get_digital_register(const std::string& name);
    std::shared_ptr<Pixel> get_pixel();

#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    cv::Mat get_transistor_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter &counter) override;
#endif

};


#endif  // SIMULATOR_PROCESSING_ELEMENT_H
