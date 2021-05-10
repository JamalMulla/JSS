//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram3t_cell.h"

#include <opencv4/opencv2/core.hpp>

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

void Dram3tCell::init() {
#ifdef TRACK_STATISTICS
    static_power_ = calc_static();
    dynamic_read_power_ = calc_dynamic_read();
    dynamic_write_power_ = calc_dynamic_write();
    time_ = this->cycle_count_ * (1.0/config_->get_clock_rate());
    scratch = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    Memory::init();
#endif
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

int Dram3tCell::get_cycle_count() {
    return cycle_count_;
}

void Dram3tCell::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Dram3tCell::print_stats(const CycleCounter& counter) {
    std::cout << "TODO: Implement in DRAM3TCELL" << std::endl;
}

void Dram3tCell::read(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, scratch);
}

void Dram3tCell::read() {
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, internal_mask);
}

void Dram3tCell::write(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, scratch);
}

void Dram3tCell::write() {
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, internal_mask);
}

int Dram3tCell::calc_transistor_count() {
    return 3;
}

double Dram3tCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

#endif

