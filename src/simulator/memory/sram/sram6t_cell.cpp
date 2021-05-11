//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/sram6t_cell.h"

#include <opencv4/opencv2/core.hpp>

#include "simulator/base/config.h"

void Sram6tCell::init() {
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
#ifdef TRACK_STATISTICS
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_read_power_ = calc_dynamic_read();
    dynamic_write_power_ = calc_dynamic_write();
    dynamic_power_ = calc_dynamic();
    width_ = calc_width();
    height_ = calc_height();
    time_ = this->cycle_count_ * (1.0/config_->get_clock_rate());
    scratch = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    this->calc_internal_mask();
#endif
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

int Sram6tCell::calc_width() {
    return 2;
}

int Sram6tCell::calc_height() {
    return 2;
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

void Sram6tCell::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Sram6tCell::print_stats(const CycleCounter& counter) {
    std::cout << "TODO: Implement in SRAM6TCELL" << std::endl;
}

int Sram6tCell::calc_transistor_count() {
    return 6;
}

double Sram6tCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

#endif
