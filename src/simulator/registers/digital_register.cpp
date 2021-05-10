//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/registers/digital_register.h"

#include <iostream>

DigitalRegister::DigitalRegister(int rows, int cols, const std::shared_ptr<Config>& config, int row_stride, int col_stride, MemoryType memory_type) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->config_ = config;
    this->type_ = CV_8U;
    this->change_memory_type(memory_type);
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister::DigitalRegister(int rows, int cols, int row_stride, int col_stride) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->type_ = CV_8U;
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister::DigitalRegister(const cv::Mat &data, int row_stride, int col_stride) {
    this->rows_ = data.rows;
    this->cols_ = data.cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->type_ = CV_8U;
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
    this->write(data);
}

DigitalRegister &DigitalRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

void DigitalRegister::set_mask(const std::shared_ptr<DigitalRegister>& mask) {
    this->mask_ = std::make_shared<cv::Mat>(mask->read());
}

cv::Mat& DigitalRegister::get_mask() {
    return *mask_;
}

void DigitalRegister::set() { this->write(1); }

void DigitalRegister::clear() { this->write(0); }

#ifdef TRACK_STATISTICS
void DigitalRegister::print_stats(const CycleCounter &counter) {

}

#endif
