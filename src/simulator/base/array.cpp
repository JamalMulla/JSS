//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/array.h"

#include <utility>

Array::Array(int rows, int columns, Config& config, ProcessingElement pe) :
    rows_(rows), columns_(columns), config_(&config), pe(std::move(pe)) {}

void Array::update_cycles(int cycles) {
    counter_ += cycles;
    this->pe.update_cycles(cycles);
}

#ifdef TRACK_STATISTICS
void Array::print_stats() {
    std::cout << "==============================" << "\n";
    std::cout << "Results" << "\n";
    std::cout << "==============================" << "\n";
    std::cout << "Clock rate: " << config_->clock_rate << " Hz\n";
    std::cout << "Process node: " << config_->process_node << " nm\n";
    std::cout << "Voltage: " << config_->voltage << " V\n";
    std::cout << "Temperature: " << config_->temperature << " C\n";
    std::cout << "==============================" << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles()/config_->clock_rate);
    std::cout << "Device execution time: " <<  exec_time << " s\n";
    int transistor_count = cv::sum(this->pe.get_transistor_count())[0];
    std::cout << "Array transistor count: " << transistor_count << "\n";
    std::cout << "Average transistor count per PE: " << transistor_count/(rows_*columns_) << " \n";
    double static_power = cv::sum(this->pe.get_static_energy())[0]/exec_time;
    std::cout << "Array static power: " <<  static_power << " W\n";
    double dynamic_power = cv::sum(this->pe.get_dynamic_energy())[0]/exec_time;
    std::cout << "Array dynamic power: " <<  dynamic_power << " W\n";
    std::cout << "Array total power: " << static_power + dynamic_power << " W\n";

    this->pe.print_stats(counter_);
}
//void Array::print_stats(const CycleCounter& counter) {
//    this->pe.print_stats(counter);
//}
//
//void Array::write_stats(const CycleCounter& counter, json& j) {
//    this->pe.write_stats(counter, j);
//}
#endif
