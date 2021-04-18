//
// Created by jm1417 on 31/01/2021.
//

#include "simulator/registers/register.h"

#include <opencv2/core.hpp>

Register::Register(int rows, int cols, int row_stride, int col_stride, int type, Config &config, MemoryType memory_type) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    config_(&config),
    value_(rows, cols, type, cv::Scalar(0)) {
    this->memory_ = Memory::construct(memory_type, rows, cols, row_stride, col_stride, config);
}

Register::Register(int rows, int cols, int row_stride, int col_stride, int type) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    value_(rows, cols, type, cv::Scalar(0)) {}


#ifdef TRACK_STATISTICS

void Register::update_static(double time) {
    if (memory_) {
        memory_->update_static(time);
    }
}

cv::Mat &Register::get_static_energy() {
    return memory_->get_static_energy();
}

cv::Mat &Register::get_dynamic_energy() {
    return memory_->get_dynamic_energy();
}

cv::Mat &Register::get_transistor_count() {
    return memory_->get_transistor_count();
}

int Register::get_cycle_count() {
    return this->memory_->get_cycle_count();
}

#endif

cv::Mat &Register::read() {
#ifdef TRACK_STATISTICS
    this->inc_read();
#endif
    return this->value_;
}

void Register::write(cv::Mat &data) {
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
    if (memory_) {
        this->memory_->read(mask);
    }
}

void Register::inc_read() {
    if (memory_) {
        this->memory_->read();
    }
}

void Register::inc_write(const cv::_InputOutputArray &mask) {
    if (memory_) {
        this->memory_->write(mask);
    }
}

void Register::inc_write() {
    if (memory_) {
        this->memory_->write();
    }
}
#endif

void Register::change_memory_type(MemoryType memory_type) {
    this->memory_ = Memory::construct(memory_type, rows_, cols_, row_stride_, col_stride_, *config_);
}

void Register::set_ui_handler(UI *ui_ptr) { this->ui = ui_ptr; }
