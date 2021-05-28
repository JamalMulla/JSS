//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/si_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

void SiCell::init() {
    process_node_ = 180;
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

int SiCell::get_cycle_count() {
    return cycle_count_;
}

void SiCell::read(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, scratch);
}

void SiCell::read() {
    read_count_++;
}

void SiCell::write(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, scratch);
}

void SiCell::write() {
    write_count_++;
}

void SiCell::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void SiCell::print_stats(const CycleCounter& counter) {
    std::cout << "TODO: Implement in SICELL" << std::endl;
}

cv::Mat SiCell::get_dynamic_energy_array() {
    cv::add(this->array_dynamic_energy_, read_count_ * this->dynamic_read_power_ * time_, this->array_dynamic_energy_, internal_mask);
    cv::add(this->array_dynamic_energy_, write_count_ * this->dynamic_write_power_ * time_, this->array_dynamic_energy_, internal_mask);

    return Component::get_dynamic_energy_array();
}

#endif
