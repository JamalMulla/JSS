//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/sram6t_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

void Sram6tCell::init() {
    process_node_ = 180;
    Memory::init();
}

#ifdef TRACK_STATISTICS

double Sram6tCell::calc_static() {
    return 1.2991e-10;  // TODO find better numbers
}

double Sram6tCell::calc_dynamic_read() {
    return 5.805e-7;
}

double Sram6tCell::calc_dynamic_write() {
    return 6.305e-7;
}

double Sram6tCell::calc_width() {
    return this->scale_width(5);
}

double Sram6tCell::calc_height() {
    return this->scale_height(5);
}

int Sram6tCell::get_cycle_count() {
    return cycle_count_;
}

int Sram6tCell::calc_transistor_count() {
    return 6;
}

double Sram6tCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

#endif
