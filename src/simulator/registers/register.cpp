//
// Created by jm1417 on 31/01/2021.
//

#include "simulator/registers/register.h"

#include <opencv2/core.hpp>

Register::Register(int rows, int columns, int type, Memory& memoryType)
    : memory_(&memoryType),
      value_(rows, columns, type, cv::Scalar(0))
       {}

void Register::update(double time) {
    memory_->update(time);
}

cv::Mat &Register::get_static_power() {
    return memory_->get_static_power();
}

cv::Mat &Register::get_dynamic_power() {
    return memory_->get_dynamic_power();
}

cv::Mat &Register::get_transistor_count() {
    return memory_->get_transistor_count();
}

int Register::get_cycle_count() {
    return this->memory_->get_cycle_count();
}

cv::Mat &Register::read() {
#ifdef TRACK_STATISTICS
    this->inc_read();
#endif
    return this->value_;
}

void Register::write(cv::Mat& data) {
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(const cv::Mat &data) {
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(Register &data) {
    this->write(data.read());
}

void Register::write(cv::Mat &data, cv::Mat &mask) {
    data.copyTo(this->value_, mask);
#ifdef TRACK_STATISTICS
    this->inc_write(mask);
#endif
}

void Register::write(Register &data, Register &mask) {
    this->write(data.read(), mask.read());
}

void Register::write(int data) {
    this->value_.setTo(data);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(int data, cv::Mat &mask) {
    this->value_.setTo(data, mask);
#ifdef TRACK_STATISTICS
    this->inc_write(mask);
#endif
}

void Register::write(int data, Register &mask) {
    this->write(data, mask.read());
}

#ifdef TRACK_STATISTICS
void Register::inc_read(const cv::_InputOutputArray &mask) {
    this->memory_->read(mask);
}

void Register::inc_write(const cv::_InputOutputArray &mask) {
    this->memory_->write(mask);
}

#endif

void Register::change_memory_type(Memory &memory_type) {
    this->memory_ = &memory_type;
}

void Register::set_ui_handler(UI *ui_ptr) { this->ui = ui_ptr; }
