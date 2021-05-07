//
// Created by jm1417 on 09/04/2021.
//
#include "simulator/adders/cla.h"

#include <opencv4/opencv2/core.hpp>

#include "../../../tests/utility.h"

/*https://www.researchgate.net/publication/283818559_A_comparative_study_of_high_speed_CMOS_adders_using_Microwind_and_FPGA
 * */

/*Bits refers to the number of bits in the two inputs and the output. So an 8-bit adders takes in two 8-bit values and outputs an 8-bit value*/


CarryLookAheadAdder::CarryLookAheadAdder(int rows, int cols, int row_stride, int col_stride, int bits, Config& config) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    cycle_count_(1),
    transistor_count_(fun_transistor(bits, config)),
    static_power_(fun_static(bits, config)),
    dynamic_power_(fun_dynamic(bits, config)),
    time_(this->cycle_count_ * (1.0 / config.get_clock_rate())),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(0)),
    array_static_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    array_dynamic_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    scratch(rows, cols, CV_8U, cv::Scalar(0)) {
    this->fun_internal_mask(rows, cols, row_stride, col_stride);
}

int CarryLookAheadAdder::fun_transistor(int bits, Config& config) {
    // Transistor count based off number of bits and config
    return 12 + 26 + ((bits - 2) * 28);
}

double CarryLookAheadAdder::fun_static(int bits, Config& config) {
    // Static power dissipation based off number of bits and config
    return 2.0e-8 * ((bits / 4.0) + 0.5); // mostly linear scaling with number of bits but also some extra for overhead
}

double CarryLookAheadAdder::fun_dynamic(int bits, Config& config) {
    // Dynamic power dissipation based off number of bits and config
    return 0.00015 * ((bits / 4.0) + 0.5);
}

void CarryLookAheadAdder::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for(int row = 0; row < rows; row += row_stride) {
        for(int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
    // todo move somewhere else
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
}

#ifdef TRACK_STATISTICS
int CarryLookAheadAdder::get_cycle_count() {
    return cycle_count_;
}

void CarryLookAheadAdder::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

cv::Mat CarryLookAheadAdder::get_static_energy() {
    return array_static_energy_;
}

cv::Mat CarryLookAheadAdder::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat CarryLookAheadAdder::get_transistor_count() {
    return array_transistor_count_;
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


