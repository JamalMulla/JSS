//
// Created by jm1417 on 17/04/2021.
//

#include "simulator/memory/dram_array.h"

Dram::Dram(int rows, int cols, int row_stride, int col_stride, int array_rows, int array_cols, int word_length, const Config& config) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    array_rows_(array_rows),
    array_cols_(array_cols),
    word_length_(word_length)
#ifdef TRACK_STATISTICS
    ,transistor_count_(fun_transistor(config)),
    static_power_(fun_static(config)),
    dynamic_power_(fun_dynamic(config)),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0)),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(0)),
    array_static_energy_(rows, cols, CV_64F, cv::Scalar(0)),
    array_dynamic_energy_(rows, cols, CV_64F, cv::Scalar(0))
#endif
{
#ifdef TRACK_STATISTICS
    this->fun_internal_mask();
#endif
    int sizes[] = {((rows*cols)/row_stride)/col_stride, array_rows, array_cols};
    data = cv::Mat(3, sizes, CV_32S, cv::Scalar(0));
}

int Dram::read(int array, int row, int col) {
    return data.at<int32_t>(array, row, col);
}

void Dram::write(int array, int row, int col, int value) {
    data.at<int32_t>(array, row, col) = value;
}

void Dram::reset() {
    data = 0;
}

#ifdef TRACK_STATISTICS
void Dram::fun_internal_mask() {
    for(int row = 0; row < rows_; row += row_stride_) {
        for(int col = 0; col < cols_; col += col_stride_) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
    // todo move somewhere else
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
}

int Dram::fun_transistor(const Config& config) {
    // 1t1c DRAM cells (for now). So rows * cols * word_length for just DRAM cell transistor counts
    // Some number for sense amplifiers
    // Some number for row/col decoders
    return (array_rows_ * array_cols_ * word_length_);
}

double Dram::fun_static(const Config& config) {
    // 1t1c DRAM cells (for now)
    // Some amount for sense amplifiers
    // Some amount for row/col decoders
    // Refresh
    return 0;
}

double Dram::fun_dynamic(const Config& config) {
    // 1t1c DRAM cells (for now)
    // Some amount for sense amplifiers
    // Some amount for row/col decoders
    // read/write entire row. dependant on word length and cols
    return 0;
}


void Dram::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}


int Dram::get_cycle_count() {
    return cycle_count_;
}


cv::Mat& Dram::get_static_energy() {
    return array_static_energy_;
}


cv::Mat& Dram::get_dynamic_energy() {
    return array_dynamic_energy_;
}


cv::Mat& Dram::get_transistor_count() {
    return array_transistor_count_;
}

void Dram::print_stats(const CycleCounter& counter) {
}
#endif
