//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/pixel.h"

#include <simulator/input/image_input.h>
#include <simulator/input/live_input.h>
#include <simulator/input/video_input.h>

#include <opencv2/imgproc.hpp>

#include <rttr/registration>
#include <utility>
#include "simulator/base/opencv_wrappers.h"

Pixel::Pixel(int rows, int cols, int row_stride, int col_stride, Source src, const std::string& path, std::shared_ptr<Config> config)
{
    set_rows(rows);
    set_cols(cols);
    set_row_stride(row_stride);
    set_col_stride(col_stride);
    set_src(src);
    set_path(path);
    set_config(std::move(config));
}

void Pixel::init() {
#ifdef TRACK_STATISTICS
    process_node_ = 180;
    Component::init();
#endif
}

void Pixel::set_path(const std::string& path) {
    this->path_ = path;
}

void Pixel::set_camera_index(int camera_index) {
    this->camera_index_ = camera_index;
}

void Pixel::set_src(Source src) {
    switch(src) {
        case LIVE: {
            input_source = std::make_shared<LiveInput>(rows_, cols_, camera_index_);
            break;
        }
        case VIDEO: {
            input_source = std::make_shared<VideoInput>(rows_, cols_, path_);
            break;
        }
        case IMAGE: {
            input_source = std::make_shared<ImageInput>(rows_, cols_, path_);
            break;
        }
    }
}

void Pixel::reset() { input_source->reset(); }

#ifdef USE_CUDA
cv::cuda::GpuMat& Pixel::read() {
#else
cv::UMat& Pixel::read() {
#endif
#ifdef TRACK_STATISTICS
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->get_clock_rate();
    ocv_wrappers::arith::add(this->array_dynamic_energy_, this->dynamic_power_, this->array_dynamic_energy_, this->internal_mask);
#endif
    return input_source->read();
}

void Pixel::read(Register& reg) {
    input_source->read(reg);
#ifdef TRACK_STATISTICS
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->get_clock_rate();
    ocv_wrappers::arith::add(this->array_dynamic_energy_, this->dynamic_power_, this->array_dynamic_energy_, this->internal_mask);
#endif
}

double Pixel::last_frame_time() {
    return input_source->last_frame_time();
}

#ifdef TRACK_STATISTICS

int Pixel::get_cycle_count() {
    return cycle_count_;
}

void Pixel::update_static(double time) {
    ocv_wrappers::arith::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Pixel::print_stats(const CycleCounter& counter) {

}

int Pixel::calc_transistor_count() {
    return 4;
}

double Pixel::calc_static() {
    return 0;
}

double Pixel::calc_dynamic() {
    return 0;
}

double Pixel::calc_width() {
    return this->scale_width(6);
}

double Pixel::calc_height() {
    return this->scale_height(9);
}

#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Pixel>("Pixel")
        .constructor()
        .method("init", &Pixel::init)
        .method("set_path", &Pixel::set_path)
        .method("set_src", &Pixel::set_src)
        .method("set_camera_index", &Pixel::set_camera_index);
};

