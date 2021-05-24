//
// Created by jm1417 on 06/03/2021.
//

#include "simulator/input/image_input.h"

#include <simulator/base/architecture.h>
#include <simulator/util/utility.h>

#include <cmath>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>

ImageInput::ImageInput(int rows, int cols, const std::string &path)
    : path_(path) {
    this->rows_ = rows;
    this->cols_ = cols;

    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if(img.empty()) {
        std::cout << "Could not read image: " << path << std::endl;
        exit(1);
    }

    std::string t_s = utility::opencv_type_to_str(img.type());
    std::cout << "Image is of type: " << t_s << std::endl;

    this->frame = cv::Mat(rows, cols, MAT_TYPE);
    this->frame.setTo(0);
}

void ImageInput::read(Register &reg) {
    reg.write(this->read());
}

cv::Mat ImageInput::read() {
    auto TIME_START = std::chrono::high_resolution_clock::now();
    cv::Mat img = cv::imread(this->path_, cv::IMREAD_GRAYSCALE);

    if(img.rows > rows_ || img.cols > cols_) {
        // TODO fix this scaling so it's dependent on which axis is bigger
        img = img(cv::Rect((cols_ - rows_) / 2, 0, cols_ - 1, cols_ - 1));
    }

    cv::resize(img, img, {cols_, rows_});

    /* img.convertTo(this->frame, MAT_TYPE, 1, -128); */
    img.convertTo(this->frame, MAT_TYPE, 1.f/255);

    auto TIME_END = std::chrono::high_resolution_clock::now();
    long time_in_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(
        TIME_END - TIME_START)
        .count();
    time_taken = time_in_nano * 1e-9;
    std::cout << "FRAME: " << this->frame << std::endl;
    return this->frame;
}

void ImageInput::reset() { this->frame.setTo(0); }

double ImageInput::last_frame_time() { return time_taken; }
