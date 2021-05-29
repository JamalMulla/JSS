//
// Created by jm1417 on 31/01/2021.
//

#include "simulator/registers/register.h"

#include <opencv2/core.hpp>


void Register::init() {
#ifdef USE_CUDA
    this->value_ = cv::cuda::GpuMat(rows_, cols_, type_, cv::Scalar(0));
#else
    this->value_ = cv::UMat(rows_, cols_, type_, cv::Scalar(0));
#endif
}

#ifdef USE_CUDA
cv::cuda::GpuMat& Register::read() {
#else
cv::UMat& Register::read() {
#endif
#ifdef TRACK_STATISTICS
    this->inc_read();
#endif
    return this->value_;
}

#ifdef USE_CUDA
void Register::write(cv::cuda::GpuMat& data) {
#else
void Register::write(cv::UMat& data) {
#endif
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

#ifdef USE_CUDA
void Register::write(const cv::cuda::GpuMat& data) {
#else
void Register::write(const cv::UMat& data) {
#endif
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}


void Register::write(const cv::Mat& data) {
    data.copyTo(this->value_);
#ifdef TRACK_STATISTICS
    this->inc_write();
#endif
}

void Register::write(Register &data) {
    this->write(data.read());
}

#ifdef USE_CUDA
void Register::write(cv::cuda::GpuMat& data, cv::cuda::GpuMat &mask) {
#else
void Register::write(cv::UMat& data, cv::UMat &mask) {
#endif
    data.copyTo(this->value_, mask);
#ifdef TRACK_STATISTICS
    this->inc_write(mask);
#endif
}

#ifdef USE_CUDA
void Register::write(cv::cuda::GpuMat& data, cv::Mat &mask) {
#else
void Register::write(cv::UMat& data, cv::Mat &mask) {
#endif
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

#ifdef USE_CUDA
void Register::write(int data, cv::cuda::GpuMat &mask) {
#else
void Register::write(int data, cv::UMat &mask) {
#endif
    this->value_.setTo(data, mask);
#ifdef TRACK_STATISTICS
    this->inc_write(mask);
#endif
}

void Register::write(int data, Register &mask) {
    this->write(data, mask.read());
}

void Register::change_memory_type(MemoryType memory_type) {
    this->memory_ = Memory::construct(memory_type, rows_, cols_, row_stride_, col_stride_, config_);
}

void Register::set_memory(MemoryType memory_type) {
    this->memory_ = Memory::construct(memory_type, rows_, cols_, row_stride_, col_stride_, config_);
}

void Register::set_type(int type) {
    this->type_ = type;
}

#ifdef TRACK_STATISTICS
void Register::update_static(double time) {
    if (memory_) {
        memory_->update_static(time);
    }
}

cv::Mat Register::get_static_energy_array() {
    if (memory_) {
        return memory_->get_static_energy_array();
    }
}

cv::Mat Register::get_dynamic_energy_array() {
    if (memory_) {
        return memory_->get_dynamic_energy_array();
    }
}

cv::Mat Register::get_transistor_count_array() {
    if (memory_) {
        return memory_->get_transistor_count_array();
    }
}

int Register::get_cycle_count() {
    return this->memory_->get_cycle_count();
}

void Register::inc_read(const cv::_InputOutputArray &mask) {
    if (memory_) {
        // mask can be noArray() which will cause an issue if we try to use it
        if (mask.empty()) {
            this->memory_->read();
        } else {
            this->memory_->read(mask);
        }
    }
}

void Register::inc_read() {
    if (memory_) {
        this->memory_->read();
    }
}

void Register::inc_write(const cv::_InputOutputArray &mask) {
    if (memory_) {
        // mask can be noArray() which will cause an issue if we try to use it
        if (mask.empty()) {
            this->memory_->write();
        } else {
            this->memory_->write(mask);
        }
    }
}

void Register::inc_write() {
    if (memory_) {
        this->memory_->write();
    }
}

double Register::get_width() {
    if (memory_) {
        return this->memory_->get_width();
    }
}

double Register::get_height() {
    if (memory_) {
        return this->memory_->get_height();
    }
}

int Register::get_transistor_count() {
    if (memory_) {
        return this->memory_->get_transistor_count();
    }
}

#endif


