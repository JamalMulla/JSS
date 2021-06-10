//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/si_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

void SiCell::init() {
    process_node_ = 180;
    Memory::init();
}

#ifdef TRACK_STATISTICS
double SiCell::calc_static() {
    return 1.2991e-12;  // TODO find better numbers
}

int SiCell::calc_transistor_count() {
    return 7;
}

double SiCell::calc_dynamic_read() {
    return 6.0e-6;
}

double SiCell::calc_dynamic_write() {
    return 6.0e-6;
}

double SiCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

double SiCell::calc_width() {
    double base = 9;
    return this->scale_width(base);
}

double SiCell::calc_height() {
    double base = 9;
    return this->scale_width(base);
}

#endif
