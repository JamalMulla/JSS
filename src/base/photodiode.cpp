//
// Created by jm1417 on 28/01/2021.
//

#include <iostream>
#include <opencv2/imgproc.hpp>
#include "photodiode.h"

Photodiode::Photodiode() : rows_(0), columns_(0) {
    this->capture = new cv::VideoCapture(0);
    this->size = new cv::Size(0, 0);
}

Photodiode::Photodiode(int rows, int columns) : rows_(rows), columns_(columns) {
    this->capture = new cv::VideoCapture(0);
    this->size = new cv::Size(columns, rows);
}

void Photodiode::reset() {
    *this->capture >> frame;
    cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2GRAY);
    cv::Mat cropFrame = this->frame(cv::Rect((this->columns_-this->rows_)/2, 0, this->columns_-1, this->rows_-1));
    cv::resize(cropFrame, cropFrame, *this->size);
    cropFrame.convertTo(this->frame, CV_16S);
}

void Photodiode::read(AnalogueRegister& reg) {
    this->frame.copyTo(reg.value());
}

Photodiode::~Photodiode() {
    delete this->capture;
    delete this->size;
}



