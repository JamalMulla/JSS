//
// Created by jm1417 on 08/02/2021.
//
#include <simulator/base/component.h>
#include <cmath>
#include <utility>

void Component::calc_internal_mask() {
    cv::Mat im = this->internal_mask.getMat(cv::ACCESS_WRITE);
    for (int row = 0; row < rows_; row += row_stride_) {
        for (int col = 0; col < cols_; col += col_stride_) {
            im.at<uint8_t>(row, col) = 1;
        }
    }
}

void Component::set_process_node(int process_node) {
    this->process_node_ = process_node;
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

cv::Mat Component::get_static_energy_array() {
#ifdef TRACK_STATISTICS
    array_static_energy_.copyTo(array_static_energy_, this->internal_mask);
#endif
    return this->array_static_energy_.getMat(cv::ACCESS_READ);
}

cv::Mat Component::get_dynamic_energy_array() {
#ifdef TRACK_STATISTICS
    array_dynamic_energy_.copyTo(array_dynamic_energy_, this->internal_mask);
#endif
    return this->array_dynamic_energy_.getMat(cv::ACCESS_READ);
}

cv::Mat Component::get_transistor_count_array() {
#ifdef TRACK_STATISTICS
    array_transistor_count_.setTo(transistor_count_, this->internal_mask);
#endif
    return this->array_transistor_count_.getMat(cv::ACCESS_READ);
}

int Component::get_transistor_count() {
    return this->transistor_count_;
}

double Component::get_width() {
    return this->width_;
}

double Component::get_height() {
    return this->height_;
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

double Component::calc_width() {
    return 0;
}

double Component::calc_height() {
    return 0;
}

double Component::scale_speed(double base) {
    double sf = 7.6 - 0.961 * log(5.531 * config_->get_clock_rate() - 104.456);
    return base * sf;
}

double Component::scale_width(double base) {
    // Scale with process node
    double sf = (double) config_->get_process_node() / this->process_node_;
    return base * sf;
}

double Component::scale_height(double base) {
    double sf = (double) config_->get_process_node() / this->process_node_;
    return base * sf;
}

#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Component>("Component")
        .method("set_process_node", &Component::set_process_node)
        .method("set_rows", &Component::set_rows)
        .method("set_cols", &Component::set_cols)
        .method("set_row_stride", &Component::set_row_stride)
        .method("set_col_stride", &Component::set_col_stride)
        .method("set_config", &Component::set_config);

};


