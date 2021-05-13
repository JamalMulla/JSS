//
// Created by jm1417 on 07/05/2021.
//

#include "simulator/adc/adc.h"
#include <rttr/registration>
#include "simulator/util/utility.h"


void ADC::init() {
#ifdef TRACK_STATISTICS
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_power_ = calc_dynamic();
    width_ = calc_width();
    height_ = calc_height();
    time_ = (this->cycle_count_ * (1.0 / config_->get_clock_rate()));
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    this->calc_internal_mask();
#endif
}

int ADC::convert(int analogue) {
   return analogue;
}

void ADC::update_dynamic(int count) {
#ifdef TRACK_STATISTICS
    cv::add(this->array_dynamic_energy_, count * this->dynamic_power_ * time_, this->array_dynamic_energy_, internal_mask);
#endif
}
#ifdef TRACK_STATISTICS

int ADC::calc_transistor_count() {
    //todo
    return 103;
}

double ADC::calc_static() {
    // todo
    return 0.000001;
}

double ADC::calc_dynamic() {
    return 0.0418; //todo find better numbers
}

double ADC::calc_width() {
    return 15;
}

double ADC::calc_height() {
    return 20;
}

void ADC::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

int ADC::get_cycle_count() {
    return cycle_count_;
}

void ADC::print_stats(const CycleCounter& counter) {
}

#endif


RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ADC>("ADC")
        .constructor()
        .method("init", &ADC::init);
};

