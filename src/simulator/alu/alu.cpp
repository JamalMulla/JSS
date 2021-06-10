//
// Created by jm1417 on 05/05/2021.
//

#include "simulator/alu/alu.h"

#include <rttr/registration>

void ALU::init() {
#ifdef TRACK_STATISTICS
    time_ = (this->cycle_count_ * (1.0 / config_->get_clock_rate()));
    Component::init();
#endif
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

double ALU::calc_width() {
    // todo
    return 10;
}

double ALU::calc_height() {
    // todo
    return 10;
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
