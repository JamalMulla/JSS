//
// Created by jm1417 on 05/05/2021.
//

#include "simulator/alu/alu.h"

#include <rttr/registration>

void ALU::init() {
#ifdef TRACK_STATISTICS
    transistor_count_ = fun_transistor(config_);
    static_power_ = fun_static(config_);
    dynamic_power_ = fun_dynamic(config_);
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    this->fun_internal_mask();
#endif
}

void ALU::set_rows(int rows) {
    this->rows_ = rows;
}

void ALU::set_cols(int cols) {
    this->cols_ = cols;
}

void ALU::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void ALU::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void ALU::set_config(const std::shared_ptr<Config>& config) {
    this->config_ = config;
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

#ifdef TRACK_STATISTICS
void ALU::fun_internal_mask() {
    for (int row = 0; row < rows_; row += row_stride_) {
        for (int col = 0; col < cols_; col += col_stride_) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
    // todo move somewhere else
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
}

int ALU::fun_transistor(const std::shared_ptr<Config>& config) {
    //todo
    return 1000;
}

double ALU::fun_static(const std::shared_ptr<Config>& config) {
    // todo
    return 0;
}

double ALU::fun_dynamic(const std::shared_ptr<Config>& config) {
    // todo
    return 55e-6;
}

void ALU::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

int ALU::get_cycle_count() {
    return cycle_count_;
}

cv::Mat ALU::get_static_energy() {
    return array_static_energy_;
}

cv::Mat ALU::get_dynamic_energy() {
    return array_dynamic_energy_;
}

cv::Mat ALU::get_transistor_count() {
    return array_transistor_count_;
}

void ALU::print_stats(const CycleCounter& counter) {
}
#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ALU>("ALU")
        .constructor()
        .method("init", &ALU::init)
        .method("set_rows", &ALU::set_rows)
        .method("set_cols", &ALU::set_cols)
        .method("set_row_stride", &ALU::set_row_stride)
        .method("set_col_stride", &ALU::set_col_stride)
        .method("set_config", &ALU::set_config);
};
