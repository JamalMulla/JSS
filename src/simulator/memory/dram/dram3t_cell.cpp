//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/dram3t_cell.h"

#include <opencv2/core.hpp>

#include "simulator/base/config.h"

// TODO take refresh into account for dynamic power? Or average out and include as part of static power.

void Dram3tCell::init() {
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
    read_count_++;
}

void Dram3tCell::write(const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, scratch);
}

void Dram3tCell::write() {
    write_count_++;
}

int Dram3tCell::calc_transistor_count() {
    return 3;
}

double Dram3tCell::calc_dynamic() {
    return calc_dynamic_read() + calc_dynamic_write();
}

cv::Mat Dram3tCell::get_dynamic_energy_array() {
    cv::add(this->array_dynamic_energy_, read_count_ * this->dynamic_read_power_ * time_, this->array_dynamic_energy_, internal_mask);
    cv::add(this->array_dynamic_energy_, write_count_ * this->dynamic_write_power_ * time_, this->array_dynamic_energy_, internal_mask);

    return Component::get_dynamic_energy_array();
}

#endif

