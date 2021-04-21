//
// Created by jm1417 on 06/03/2021.
//

#include "simulator/input/video_input.h"

#include <simulator/base/plane_params.h>

#include "opencv4/opencv2/videoio.hpp"

VideoInput::VideoInput(int rows, int cols, const std::string &path) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->capture = std::make_unique<cv::VideoCapture>(path);
    if(!this->capture->isOpened()) {
        std::cerr << "Could not open video: " << path << std::endl;
        exit(1);
    }
    this->size = std::make_unique<cv::Size>(cols, rows);
    this->frame = cv::Mat(rows, cols, MAT_TYPE);
    this->frame.setTo(0);
}

void VideoInput::read(Register &reg) {
    LiveInput::read(reg);
    frame_count++;
    if(frame_count == this->capture->get(cv::CAP_PROP_FRAME_COUNT)) {
        frame_count = 0;
        this->capture->set(cv::CAP_PROP_POS_FRAMES, 0);
    }
}
