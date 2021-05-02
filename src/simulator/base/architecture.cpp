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
    return components_[name];
}

void Architecture::add_component(const std::string& name, std::shared_ptr<Component> component) {
    components_[name] = std::move(component);
}

void Architecture::add_components(std::unordered_map<std::string, std::shared_ptr<Component>> components) {
    components_.merge(components);
}

void Architecture::update_cycles(int cycles) {
#ifdef TRACK_STATISTICS
    counter_ += cycles;
#endif
}

#ifdef TRACK_STATISTICS

void Architecture::update_static() {
    double time = (1.0 / config_.clock_rate_) * counter_.get_cycles();
    for (auto& [_, component] : components_) {
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
    std::cout << "Clock rate: " << config_.clock_rate_ << " Hz\n";
    std::cout << "Process node: " << config_.process_node_ << " nm\n";
    std::cout << "Voltage: " << config_.voltage_ << " V\n";
    std::cout << "Temperature: " << config_.temperature_ << " C\n";
    std::cout << "=============================="
              << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles() / config_.clock_rate_);
    std::cout << "Device execution time: " << exec_time << " s\n";
//    std::cout << "FPS: " << 500/exec_time << " \n";

    long transistor_count = 0;
    for (auto& [_, component] : components_) {
        transistor_count += cv::sum(component->get_transistor_count())[0];
    }
    std::cout << "Architecture transistor count: " << transistor_count << "\n";
//    std::cout << "Average transistor count per PE: " << transistor_count / (rows_ * columns_) << " \n";

    double static_energy = 0.0;
    for (auto& [_, component] : components_) {
        static_energy += cv::sum(component->get_static_energy())[0];
    }
    double static_power = static_energy/exec_time;

    double dynamic_energy = 0.0;
    for (auto& [_, component] : components_) {
        dynamic_energy += cv::sum(component->get_dynamic_energy())[0];
    }
    double dynamic_power = dynamic_energy/exec_time;

    std::cout << "Architecture static energy: " << static_energy << " J\n";
    std::cout << "Architecture static power: " << static_power << " W\n";
    std::cout << "Architecture dynamic energy: " << dynamic_energy << " J\n";
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
