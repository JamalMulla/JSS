//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram_3t_cell.h"

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

Dram3tCell::Dram3tCell(Config& config) :
    static_power_(fun_static(config)),
    dynamic_read_power_(fun_dynamic_read(config)),
    dynamic_write_power_(fun_dynamic_write(config)),
    config_(&config) {
}

double Dram3tCell::fun_static(const Config& config) {
    return 2.17e-8; // TODO find better numbers
}

double Dram3tCell::fun_dynamic_read(const Config& config) {
    return 5.8180745e-7;
}

double Dram3tCell::fun_dynamic_write(const Config& config) {
    return 2.4934605e-7;
}

double Dram3tCell::get_static_power(double time) {
    return static_power_;
}

double Dram3tCell::get_dynamic_power() {
    return dynamic_power_;
}

int Dram3tCell::get_cycle_count() {
    return cycle_count_;
}

int Dram3tCell::get_transistor_count() {
    return transistor_count_;
}

void Dram3tCell::read() {
    this->dynamic_power_ = this->dynamic_read_power_;
}

void Dram3tCell::write() {
    this->dynamic_power_ = this->dynamic_write_power_;
}
