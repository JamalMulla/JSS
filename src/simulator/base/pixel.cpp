//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/pixel.h"

#include <simulator/input/image_input.h>
#include <simulator/input/live_input.h>
#include <simulator/input/video_input.h>

#include <opencv2/imgproc.hpp>

Pixel::Pixel(int rows, int cols, int row_stride, int col_stride, Source src, const std::string& path, Config& config) :
    cycle_count_(0),
    transistor_count_(4),
    static_power_(0),
    dynamic_power_(0),
    array_transistor_count_(rows, cols, CV_32S, cv::Scalar(transistor_count_)),
    array_static_power_(rows, cols, CV_32F, cv::Scalar(0)),
    array_dynamic_power_(rows, cols, CV_32F, cv::Scalar(0)),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0)),
    config_(&config) {
    switch(src) {
        case LIVE: {
            input_source = std::make_shared<LiveInput>(rows, cols);
            break;
        }
        case VIDEO: {
            input_source = std::make_shared<VideoInput>(rows, cols, path);
            break;
        }
        case IMAGE: {
            input_source = std::make_shared<ImageInput>(rows, cols, path);
            break;
        }
    }
    this->fun_internal_mask(rows, cols, row_stride, col_stride);
}

void Pixel::reset() { input_source->reset(); }

void Pixel::read(Register& reg) {
    input_source->read(reg);
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->clock_rate;
    cv::add(this->array_dynamic_power_, this->dynamic_power_, this->array_dynamic_power_, this->internal_mask);
}

double Pixel::last_frame_time() {
    return input_source->last_frame_time();
}

#ifdef TRACK_STATISTICS

cv::Mat& Pixel::get_static_power() {
    return array_static_power_;
}

cv::Mat& Pixel::get_dynamic_power() {
    return array_dynamic_power_;
}

int Pixel::get_cycle_count() {
    return cycle_count_;
}

cv::Mat& Pixel::get_transistor_count() {
    return array_transistor_count_;
}
void Pixel::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for(int row = 0; row < rows; row += row_stride) {
        for(int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
}
void Pixel::update(double time) {
    cv::add(this->array_static_power_, this->static_power_ * time, this->array_static_power_, this->internal_mask);
}

#endif

Pixel::~Pixel() = default;
