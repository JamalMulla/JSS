//
// Created by jm1417 on 09/04/2021.
//
#include "simulator/adders/cla.h"

#include <opencv2/core.hpp>

/*https://www.researchgate.net/publication/283818559_A_comparative_study_of_high_speed_CMOS_adders_using_Microwind_and_FPGA
 * */

CarryLookAheadAdder::CarryLookAheadAdder(int rows, int cols, int row_stride, int col_stride, int bits, const Config& config) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    cycle_count_(1),
    transistor_count_(fun_transistor(bits, config)),
    static_power_(fun_static(bits, config)),
    dynamic_power_(fun_dynamic(bits, config)),
    time_(this->cycle_count_ * (1.0/config.clock_rate)),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(transistor_count_)),
    array_static_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    array_dynamic_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    scratch(rows, cols, CV_8U, cv::Scalar(0)) {}

int CarryLookAheadAdder::fun_transistor(int bits, const Config& config) {
    // Transistor count based off number of bits and config
    return 12 + 26 + ((bits - 2) * 28);
}

double CarryLookAheadAdder::fun_static(int bits, const Config& config) {
    // Static power dissipation based off number of bits and config
    return 2.0e-5;
}

double CarryLookAheadAdder::fun_dynamic(int bits, const Config& config) {
    // Dynamic power dissipation based off number of bits and config
    return 0.00015;
}

void CarryLookAheadAdder::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for(int row = 0; row < rows; row += row_stride) {
        for(int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
}

#ifdef TRACK_STATISTICS
int CarryLookAheadAdder::get_cycle_count() {
    return cycle_count_;
}

void CarryLookAheadAdder::update(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_);
}

cv::Mat& CarryLookAheadAdder::get_static_energy() {
    return array_static_energy_;
}

cv::Mat& CarryLookAheadAdder::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat& CarryLookAheadAdder::get_transistor_count() {
    return array_transistor_count_;
}
#endif

void CarryLookAheadAdder::add(DigitalRegister& dst, DigitalRegister& src1, DigitalRegister& src2, const cv::_InputOutputArray& mask) {
    scratch = 0;
    cv::bitwise_and(this->internal_mask, mask, scratch);
    dst.write(src1.read() + src2.read());
    cv::add(this->array_dynamic_energy_, this->dynamic_power_ * time_, this->array_dynamic_energy_, scratch);
}
