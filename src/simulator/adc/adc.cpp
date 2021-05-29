//
// Created by jm1417 on 07/05/2021.
//

#include "simulator/adc/adc.h"
#include <rttr/registration>
#include "simulator/util/utility.h"

void ADC::init() {
#ifdef TRACK_STATISTICS
    time_ = (this->cycle_count_ * (1.0 / config_->get_clock_rate()));
    Component::init();
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

