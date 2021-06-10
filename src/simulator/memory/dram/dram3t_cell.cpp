//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram3t_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

void Dram3tCell::init() {
    process_node_ = 180;
    Memory::init();
}

#ifdef TRACK_STATISTICS
double Dram3tCell::calc_static() {
    return 2.17e-10;  // TODO find better numbers
}

double Dram3tCell::calc_dynamic_read() {
    return 5.8180745e-6;
}

double Dram3tCell::calc_dynamic_write() {
    return 2.4934605e-6;
}

double Dram3tCell::calc_width() {
    return this->scale_width(3);
}

double Dram3tCell::calc_height() {
    return this->scale_height(3);
}

int Dram3tCell::get_cycle_count() {
    return cycle_count_;
}

int Dram3tCell::calc_transistor_count() {
    return 3;
}

double Dram3tCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

#endif
