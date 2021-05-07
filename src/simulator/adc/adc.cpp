//
// Created by jm1417 on 07/05/2021.
//

#include "simulator/adc/adc.h"
#include <rttr/registration>


void ADC::init() {
#ifdef TRACK_STATISTICS
    transistor_count_ = fun_transistor(config_);
    static_power_ = fun_static(config_);
    dynamic_power_ = fun_dynamic(config_);
    time_ = (this->cycle_count_ * (1.0 / config_->get_clock_rate()));
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    this->fun_internal_mask();
#endif
}

void ADC::set_rows(int rows) {
    this->rows_ = rows;
}

void ADC::set_cols(int cols) {
    this->cols_ = cols;
}

void ADC::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void ADC::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void ADC::set_config(const std::shared_ptr<Config>& config) {
    this->config_ = config;
}

int ADC::convert(int analogue) {
   return analogue;
}


#ifdef TRACK_STATISTICS
void ADC::fun_internal_mask() {
    for (int row = 0; row < rows_; row += row_stride_) {
        for (int col = 0; col < cols_; col += col_stride_) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
    // todo move somewhere else
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
}

int ADC::fun_transistor(const std::shared_ptr<Config>& config) {
    //todo
    return 103;
}

double ADC::fun_static(const std::shared_ptr<Config>& config) {
    // todo
    return 0.000001;
}

double ADC::fun_dynamic(const std::shared_ptr<Config>& config) {
    return 0.0418; //todo find better numbers
}

void ADC::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

int ADC::get_cycle_count() {
    return cycle_count_;
}

cv::Mat ADC::get_static_energy() {
    return array_static_energy_;
}

cv::Mat ADC::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat ADC::get_transistor_count() {
    return array_transistor_count_;
}

void ADC::print_stats(const CycleCounter& counter) {
}

void ADC::update_dynamic(int count) {
    cv::add(this->array_dynamic_energy_, count * this->dynamic_power_ * time_, this->array_dynamic_energy_, internal_mask);
}
#endif


RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ADC>("ADC")
        .constructor()
        .method("init", &ADC::init)
        .method("set_rows", &ADC::set_rows)
        .method("set_cols", &ADC::set_cols)
        .method("set_row_stride", &ADC::set_row_stride)
        .method("set_col_stride", &ADC::set_col_stride)
        .method("set_config", &ADC::set_config);
};

