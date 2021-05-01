//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/architecture.h"

#include <utility>

//Architecture::Architecture(int rows, int columns, Config& config, ProcessingElement pe) :
//    rows_(rows), columns_(columns),
//    config_(config), pe(std::move(pe))
////    cla(rows, columns, 4, 4, 8, config),
////    dram(rows, columns, 8, 8, 256, 1, 16, config)
//{}

std::shared_ptr<Component> Architecture::get_component(const std::string& name) {
    return components[name];
}

void Architecture::add_component(const std::string& name, std::shared_ptr<Component> component) {
    components[name] = std::move(component);
}

void Architecture::update_cycles(int cycles) {
#ifdef TRACK_STATISTICS
    counter_ += cycles;
#endif
}

#ifdef TRACK_STATISTICS

void Architecture::update_static() {
    double time = (1.0 / config_.clock_rate) * counter_.get_cycles();
    for (auto& [_, component] : components) {
        component->update_static(time);
    }
}

void Architecture::print_stats() {

    std::cout << "=============================="
              << "\n";
    std::cout << "Results"
              << "\n";
    std::cout << "=============================="
              << "\n";
    std::cout << "Clock rate: " << config_.clock_rate << " Hz\n";
    std::cout << "Process node: " << config_.process_node << " nm\n";
    std::cout << "Voltage: " << config_.voltage << " V\n";
    std::cout << "Temperature: " << config_.temperature << " C\n";
    std::cout << "=============================="
              << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles() / config_.clock_rate);
    std::cout << "Device execution time: " << exec_time << " s\n";
//    std::cout << "FPS: " << 500/exec_time << " \n";
    int pe_tc = cv::sum(this->pe.get_transistor_count())[0];
//    int cla_tc =  cv::sum(this->cla.get_transistor_count())[0];
//    int dram_tc = cv::sum(this->dram.get_transistor_count())[0];
    int transistor_count =  pe_tc + 0 + 0;
    std::cout << "Architecture transistor count: " << transistor_count << "\n";
    std::cout << "Average transistor count per PE: " << transistor_count / (rows_ * columns_) << " \n";
    double static_power = (cv::sum(this->pe.get_static_energy())[0]) / exec_time;
    std::cout << "Architecture static power: " << static_power << " W\n";
    double dynamic_power = (cv::sum(this->pe.get_dynamic_energy())[0]) / exec_time;
    std::cout << "Architecture dynamic power: " << dynamic_power << " W\n";
    std::cout << "Architecture total power: " << static_power + dynamic_power << " W\n";

//    this->pe.print_stats(counter_);
}

unsigned long long Architecture::get_cycles() {
    return counter_.get_cycles();
}
//void Architecture::print_stats(const CycleCounter& counter) {
//    this->pe.print_stats(counter);
//}
//
//void Architecture::write_stats(const CycleCounter& counter, json& j) {
//    this->pe.write_stats(counter, j);
//}
#endif
