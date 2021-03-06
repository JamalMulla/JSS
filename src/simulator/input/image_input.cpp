//
// Created by jm1417 on 06/03/2021.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <simulator/base/array.h>
#include <opencv2/opencv.hpp>
#include <simulator/util/utility.h>
#include <cmath>
#include "simulator/input/image_input.h"

ImageInput::ImageInput(int rows, int cols, const std::string &path) {
    this->rows_ = rows;
    this->cols_ = cols;

    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    if (img.empty()) {
        std::cout << "Could not read the image: " << path << std::endl;
        exit(1);
    }

    std::string t_s = utility::opencv_type_to_str(img.type());
    std::cout << "Image is of type: " << t_s << std::endl;

    this->frame = cv::Mat(rows, cols, MAT_TYPE);
    this->reset();

    auto TIME_START = std::chrono::high_resolution_clock::now();

    if (img.rows > rows_ || img.cols > cols_) {
        //TODO fix this scaling so it's dependent on which axis is bigger
        img = img(cv::Rect((cols_ - rows_)/2, 0, cols_-1, cols-1));
    }

    cv::resize(img, img, {cols, rows});

    img.convertTo(this->frame, MAT_TYPE);

    auto TIME_END = std::chrono::high_resolution_clock::now();
    long time_in_nano = std::chrono::duration_cast<std::chrono::nanoseconds>(TIME_END-TIME_START).count();
    time_taken = time_in_nano*1e-9;
}

void ImageInput::read(Register &reg) {
    this->frame.copyTo(reg.value());
}

void ImageInput::reset() {
    // TODO should we do anything here?
}

double ImageInput::last_frame_time() {
    return time_taken;
}