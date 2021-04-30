//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram3t_cell.h"

#include <opencv4/opencv2/core.hpp>

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

Dram3tCell::Dram3tCell(int rows, int cols, int row_stride, int col_stride, Config& config) :
    Memory(rows, cols, row_stride, col_stride)
#ifdef TRACK_STATISTICS
    ,
    static_power_(fun_static(config)),
    dynamic_read_power_(fun_dynamic_read(config)),
    dynamic_write_power_(fun_dynamic_write(config)),
    config_(config),
    time_(this->cycle_count_ * (1.0/config_.clock_rate)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(transistor_count_)),
    array_static_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    array_dynamic_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    scratch(rows, cols, CV_8U, cv::Scalar(0))
#endif
{}

#ifdef TRACK_STATISTICS
double Dram3tCell::fun_static(const Config& config) {
    return 2.17e-10;  // TODO find better numbers
}

double Dram3tCell::fun_dynamic_read(const Config& config) {
    return 5.8180745e-6;
}

double Dram3tCell::fun_dynamic_write(const Config& config) {
    return 2.4934605e-6;
}

cv::Mat Dram3tCell::get_static_energy() {
    return array_static_energy_;
}

cv::Mat Dram3tCell::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat Dram3tCell::get_transistor_count() {
    return array_transistor_count_;
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
#endif

