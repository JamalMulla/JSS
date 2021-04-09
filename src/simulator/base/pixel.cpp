//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/pixel.h"

#include <simulator/input/image_input.h>
#include <simulator/input/live_input.h>
#include <simulator/input/video_input.h>

#include <opencv2/imgproc.hpp>

Pixel::Pixel(int rows, int cols, Source src, const std::string& path, Config& config) :
    cycle_count_(0),
    transistor_count_(4),
    static_power_(0),
    dynamic_power_(0),
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
}

void Pixel::reset() { input_source->reset(); }

void Pixel::read(Register& reg) {
    input_source->read(reg);
    double seconds = this->input_source->last_frame_time();
    cycle_count_ = seconds * this->config_->clock_rate;
}

double Pixel::last_frame_time() { return input_source->last_frame_time(); }

#ifdef TRACK_STATISTICS

double Pixel::get_static_power(double time) {
    return static_power_ * time;
}

double Pixel::get_dynamic_power() {
    return dynamic_power_;
}

int Pixel::get_cycle_count() {
    return cycle_count_;
}

int Pixel::get_transistor_count() {
    return transistor_count_;
}

void Pixel::print_stats(const CycleCounter& counter) {}

void Pixel::write_stats(const CycleCounter& counter, json& j) {}
#endif

Pixel::~Pixel() = default;
