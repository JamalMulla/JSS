//
// Created by jm1417 on 28/01/2021.
//

#include <opencv2/imgproc.hpp>
#include <simulator/input/live_input.h>
#include <simulator/input/image_input.h>
#include "simulator/base/photodiode.h"
#include <stdexcept>
#include <simulator/input/video_input.h>

Photodiode::Photodiode(int rows, int cols, Source src, const std::string& path) {
    switch (src) {
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

void Photodiode::reset() {
    input_source->reset();
}

void Photodiode::read(Register& reg) {
    input_source->read(reg);
}

double Photodiode::last_frame_time() {
    return input_source->last_frame_time();
}

void Photodiode::print_stats(const CycleCounter &counter) {

}

void Photodiode::write_stats(const CycleCounter &counter, json& j) {

}

Photodiode::~Photodiode() {
}



