//
// Created by jm1417 on 05/05/2021.
//

#include "simulator/alu/alu.h"

#include <rttr/registration>

void ALU::init() {
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
#ifdef TRACK_STATISTICS
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_power_ = calc_dynamic();
    time_ = (this->cycle_count_ * (1.0 / config_->get_clock_rate()));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
#endif
    this->calc_internal_mask();
}

void ALU::set_bits(int bits) {
    this->bits_ = bits;
}

// todo all flags
int ALU::execute(int a, int b, ALU::OPCODE opcode) {
    N = false;
    switch (opcode) {
        case ADD: {
            return a + b;
        };
        case SUB: {
            return a - b;
        };
        case INC: {
            return a + 1;
        };
        case DEC: {
            return a - 1;
        };
        case CMP: {
            int v = a - b;
            if (v < 0) {
                N = true;
            }
            return -1;
        };
        case MUL: {
            return a * b;
        };
        case DIV: {
            return a / b;
        }
        case AND: {
            return a & b;
        };
        case OR: {
            return a | b;
        };
        case XOR: {
            return a ^ b;
        };
    }
}

void ALU::update_dynamic(int count) {
#ifdef TRACK_STATISTICS
    cv::add(this->array_dynamic_energy_, count * this->dynamic_power_ * time_, this->array_dynamic_energy_, internal_mask);
#endif
}

#ifdef TRACK_STATISTICS
int ALU::calc_transistor_count() {
    //todo
    return 1000;
}

double ALU::calc_static() {
    // todo
    return 0.000001;
}

double ALU::calc_dynamic() {
    return 0.000751428;
}

void ALU::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

int ALU::get_cycle_count() {
    return cycle_count_;
}

void ALU::print_stats(const CycleCounter& counter) {
}
#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ALU>("ALU")
        .constructor()
        .method("init", &ALU::init)
        .method("set_bits", &ALU::set_bits);
};
