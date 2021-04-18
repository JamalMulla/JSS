//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/array.h"

#include <utility>

void Array::update_cycles(int cycles) {
#ifdef TRACK_STATISTICS
    counter_ += cycles;
#endif
}

Array::Array(int rows, int columns, Config& config, ProcessingElement pe) :
    rows_(rows), columns_(columns), config_(&config), pe(std::move(pe)), cla(rows, columns, 4, 4, 8, config) {}

#ifdef TRACK_STATISTICS

void Array::update_static() {
    double time = (1.0 / config_->clock_rate) * counter_.get_cycles();
    this->pe.update_static(counter_.get_cycles());
    this->cla.update_static(time);
}

void Array::print_stats() {

    std::cout << "=============================="
              << "\n";
    std::cout << "Results"
              << "\n";
    std::cout << "=============================="
              << "\n";
    std::cout << "Clock rate: " << config_->clock_rate << " Hz\n";
    std::cout << "Process node: " << config_->process_node << " nm\n";
    std::cout << "Voltage: " << config_->voltage << " V\n";
    std::cout << "Temperature: " << config_->temperature << " C\n";
    std::cout << "=============================="
              << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles() / config_->clock_rate);
    std::cout << "Device execution time: " << exec_time << " s\n";
    std::cout << "FPS: " << 500/exec_time << " \n";
    int transistor_count = cv::sum(this->pe.get_transistor_count())[0] + cv::sum(this->cla.get_transistor_count())[0];
    std::cout << "Array transistor count: " << transistor_count << "\n";
    std::cout << "Average transistor count per PE: " << transistor_count / (rows_ * columns_) << " \n";
    double static_power = (cv::sum(this->pe.get_static_energy())[0] + cv::sum(this->cla.get_static_energy())[0]) / exec_time;
    std::cout << "Array static power: " << static_power << " W\n";
    double dynamic_power = (cv::sum(this->pe.get_dynamic_energy())[0] + cv::sum(this->cla.get_dynamic_energy())[0]) / exec_time;
    std::cout << "Array dynamic power: " << dynamic_power << " W\n";
    std::cout << "Array total power: " << static_power + dynamic_power << " W\n";

//    this->pe.print_stats(counter_);
}

unsigned long long Array::get_cycles() {
    return counter_.get_cycles();
}
//void Array::print_stats(const CycleCounter& counter) {
//    this->pe.print_stats(counter);
//}
//
//void Array::write_stats(const CycleCounter& counter, json& j) {
//    this->pe.write_stats(counter, j);
//}
#endif
