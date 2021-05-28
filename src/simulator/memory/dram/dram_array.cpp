//
// Created by jm1417 on 17/04/2021.
//

#include "simulator/memory/dram_array.h"

#include <rttr/registration>

void Dram::init() {
#ifdef TRACK_STATISTICS
    transistor_count_ = calc_transistor_count();
    static_power_ = calc_static();
    dynamic_power_ = calc_dynamic();
    width_ = calc_width();
    height_ = calc_height();
    time_ = this->cycle_count_ * (1.0 / config_->get_clock_rate());
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    this->calc_internal_mask();
#endif
    // dims = (arrays x rows_in_array x cols_in_row)
    int sizes[] = {((rows_ * cols_) / (row_stride_ * col_stride_)), array_rows_, array_cols_};
    data = cv::Mat(3, sizes, CV_8U, cv::Scalar(0)); // only holds 1s and 0s
}


void Dram::set_array_rows(int array_rows) {
    this->array_rows_ = array_rows;
}

void Dram::set_array_cols(int array_cols) {
    this->array_cols_ = array_cols;
}

int Dram::read_signed_int(int array, int row, int start_col) {
    int32_t value = 0;
    for (int i = 0; i < 32; i++) {
        bool b = read_bit(array, row, start_col + i);
        value |= b << i;
    }

    return value;
}

void Dram::write_signed_int(int array, int row, int start_col, int value) {
    if (value < -2147483648 ) {
        value = -2147483648;
    } else if (value > 2147483647) {
        value = 2147483647;
    }
    for (int i = 0; i < 32; i++) {
        write_bit(array, row, start_col + i, value & (1 << i));
    }
}

int Dram::read_signed_short(int array, int row, int start_col) {
    int16_t value = 0;
    for (int i = 0; i < 16; i++) {
        bool b = read_bit(array, row, start_col + i);
        value |= b << i;
    }

    return value;
}

void Dram::write_signed_short(int array, int row, int start_col, int value) {
    if (value < -32768 ) {
        value = -32768;
    } else if (value > 32767) {
        value = 32767;
    }
    for (int i = 0; i < 16; i++) {
        write_bit(array, row, start_col + i, value & (1 << i));
    }
}

int Dram::read_signed_byte(int array, int row, int start_col) {
    int8_t value = 0;
    for (int i = 0; i < 8; i++) {
        bool b = read_bit(array, row, start_col + i);
        value |= b << i;
    }

    return value;
}

void Dram::write_signed_byte(int array, int row, int start_col, int value) {
    if (value < -128 ) {
        value = -128;
    } else if (value > 127) {
        value = 127;
    }
    for (int i = 0; i < 8; i++) {
        write_bit(array, row, start_col + i, value & (1 << i));
    }
}

bool Dram::read_bit(int array, int row, int col) {
    // todo dynamic energy costs
    return data.at<uint8_t>(array, row, col);
}

void Dram::write_bit(int array, int row, int col, bool value) {
    // todo dynamic energy costs
    data.at<uint8_t>(array, row, col) = value;
}

void Dram::print_row(int array, int row) {
    for (int col = 0; col < array_cols_; ++col) {
        bool b = read_bit(array, row, col);
        std::cout << (int) b;
    }
    std::cout << std::endl;
}

void Dram::reset() {
    data = 0;
}

void Dram::update_dynamic(int count) {
#ifdef TRACK_STATISTICS
    cv::add(this->array_dynamic_energy_, count * this->dynamic_power_ * time_, this->array_dynamic_energy_, internal_mask);
#endif
}

#ifdef TRACK_STATISTICS

int Dram::calc_transistor_count() {
    // 1t1c DRAM cells (for now). So rows * cols for just DRAM cell transistor counts
    // Some number for sense amplifiers
    // Some number for row/col decoders
    return (array_rows_ * array_cols_) + (2 * array_cols_);
}

double Dram::calc_static() {
    // 1t1c DRAM cells (for now)
    // Some amount for sense amplifiers
    // Some amount for row/col decoders
    // Refresh
    return 5.0e-11;
}

double Dram::calc_dynamic() {
    // 1t1c DRAM cells (for now)
    // Some amount for sense amplifiers
    // Some amount for row/col decoders
    // read/write entire row. dependant on word length and cols
    return 5.0e-9;
}

double Dram::calc_width() {
    return array_cols_ / 15;
}

double Dram::calc_height() {
    return array_rows_ / 15;
}

void Dram::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

int Dram::get_cycle_count() {
    return cycle_count_;
}

void Dram::print_stats(const CycleCounter& counter) {
}

#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Dram>("Dram")
        .constructor()
        .method("init", &Dram::init)
        .method("set_array_rows", &Dram::set_array_rows)
        .method("set_array_cols", &Dram::set_array_cols);
};
