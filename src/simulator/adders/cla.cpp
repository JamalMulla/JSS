//
// Created by jm1417 on 09/04/2021.
//
#include "simulator/adders/cla.h"
#include <rttr/registration>
#include <opencv2/core.hpp>


/*https://www.researchgate.net/publication/283818559_A_comparative_study_of_high_speed_CMOS_adders_using_Microwind_and_FPGA
 * */

/*Bits refers to the number of bits in the two inputs and the output. So an 8-bit adders takes in two 8-bit values and outputs an 8-bit value*/

void CarryLookAheadAdder::init() {
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
#ifdef TRACK_STATISTICS
    cycle_count_ = 1;
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_power_ = calc_dynamic();
    width_ = calc_width();
    height_ = calc_height();
    time_ = this->cycle_count_ * (1.0 / config_->get_clock_rate());
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    scratch  = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
#endif
    this->calc_internal_mask();
}

#ifdef TRACK_STATISTICS
int CarryLookAheadAdder::calc_transistor_count() {
    // Transistor count based off number of bits and config
    return 12 + 26 + ((bits_ - 2) * 28);
}

double CarryLookAheadAdder::calc_static() {
    // Static power dissipation based off number of bits and config
    return 2.0e-8 * ((bits_ / 4.0) + 0.5); // mostly linear scaling with number of bits but also some extra for overhead
}

double CarryLookAheadAdder::calc_dynamic() {
    // Dynamic power dissipation based off number of bits and config
    return 0.00015 * ((bits_ / 4.0) + 0.5);
}

int CarryLookAheadAdder::get_cycle_count() {
    return cycle_count_;
}

void CarryLookAheadAdder::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

double CarryLookAheadAdder::calc_width() {
    return (12 + 26 + ((bits_ - 2) * 28)) / 10.0;
}

double CarryLookAheadAdder::calc_height() {
    return (12 + 26 + ((bits_ - 2) * 28)) / 10.0;
}

void CarryLookAheadAdder::inc_add() {
    cv::add(this->array_dynamic_energy_, this->dynamic_power_ * time_, this->array_dynamic_energy_, internal_mask);
}

void CarryLookAheadAdder::print_stats(const CycleCounter& counter) {
}
#endif

int CarryLookAheadAdder::add(int src1, int src2) {
    return src1 + src2;
}

void CarryLookAheadAdder::set_bits(int bits) {
    this->bits_ = bits;
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<CarryLookAheadAdder>("CLA")
        .constructor()
        .method("init", &CarryLookAheadAdder::init)
        .method("set_bits", &CarryLookAheadAdder::set_bits);

};
