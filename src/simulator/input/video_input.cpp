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
#ifdef USE_CUDA
    this->frame = cv::cuda::GpuMat(rows, cols, MAT_TYPE);
#else
    this->frame = cv::UMat(rows, cols, MAT_TYPE);
#endif
    this->frame.setTo(0);
}

#ifdef USE_CUDA
cv::cuda::GpuMat& VideoInput::read() {
#else
cv::UMat& VideoInput::read() {
#endif
    LiveInput::read();
    frame_count++;
    if(frame_count == this->capture->get(cv::CAP_PROP_FRAME_COUNT)) {
        frame_count = 0;
        this->capture->set(cv::CAP_PROP_POS_FRAMES, 0);
    }
    return this->frame;
}

void VideoInput::read(Register &reg) {
    reg.write(this->read());
}
