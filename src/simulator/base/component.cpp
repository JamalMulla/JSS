//
// Created by jm1417 on 08/02/2021.
//
#include <simulator/base/component.h>

#include <utility>

void Component::calc_internal_mask() {
    for (int row = 0; row < rows_; row += row_stride_) {
        for (int col = 0; col < cols_; col += col_stride_) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
#ifdef TRACK_STATISTICS
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
#endif
}

void Component::set_rows(int rows) {
    this->rows_ = rows;
}

void Component::set_cols(int cols) {
    this->cols_ = cols;
}

void Component::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void Component::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void Component::set_config(std::shared_ptr<Config> config) {
    this->config_ = std::move(config);
}

#ifdef TRACK_STATISTICS

cv::Mat Component::get_static_energy() {
    return this->array_static_energy_;
}

cv::Mat Component::get_dynamic_energy() {
    return this->array_dynamic_energy_;
}

cv::Mat Component::get_transistor_count() {
    return this->array_transistor_count_;
}

int Component::calc_transistor_count() {
    return 0;
}

double Component::calc_static() {
    return 0;
}

double Component::calc_dynamic() {
    return 0;
}

#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Component>("Component")
        .method("set_rows", &Component::set_rows)
        .method("set_cols", &Component::set_cols)
        .method("set_row_stride", &Component::set_row_stride)
        .method("set_col_stride", &Component::set_col_stride)
        .method("set_config", &Component::set_config);

};


