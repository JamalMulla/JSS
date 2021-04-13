//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/sram6t_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

Sram6tCell::Sram6tCell(int rows, int cols, int row_stride, int col_stride, Config& config) :
    Memory(rows, cols, row_stride, col_stride)
#ifdef TRACK_STATISTICS
    ,
    static_power_(fun_static(config)),
    dynamic_read_power_(fun_dynamic_read(config)),
    dynamic_write_power_(fun_dynamic_write(config)),
    config_(&config),
    time_(this->cycle_count_ * (1.0/config_->clock_rate)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(transistor_count_)),
    array_static_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    array_dynamic_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    scratch(rows, cols, CV_8U, cv::Scalar(0))
#endif
{}

#ifdef TRACK_STATISTICS

double Sram6tCell::fun_static(const Config& config) {
    return 1.2991e-10;  // TODO find better numbers
}

double Sram6tCell::fun_dynamic_read(const Config& config) {
    return 5.805e-8;
}

double Sram6tCell::fun_dynamic_write(const Config& config) {
    return 6.305e-8;
}

cv::Mat& Sram6tCell::get_static_energy() {
    return array_static_energy_;
}

cv::Mat& Sram6tCell::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat& Sram6tCell::get_transistor_count() {
    return array_transistor_count_;
}

int Sram6tCell::get_cycle_count() {
    return cycle_count_;
}

void Sram6tCell::read(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, scratch);
}

void Sram6tCell::read() {
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, internal_mask);
}

void Sram6tCell::write(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, scratch);
}

void Sram6tCell::write() {
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, internal_mask);
}

void Sram6tCell::update(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Sram6tCell::print_stats(const CycleCounter& counter) {
    std::cout << "TODO: Implement in SRAM6TCELL" << std::endl;
}

#endif
