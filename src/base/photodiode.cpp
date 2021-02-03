//
// Created by jm1417 on 28/01/2021.
//

#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "photodiode.h"

Photodiode::Photodiode(int rows, int columns) : rows_(rows), columns_(columns) {
    this->capture = std::make_shared<cv::VideoCapture>(0,  cv::CAP_ANY);
    if (!(*this->capture).isOpened()) {
        std::cerr << "Could not open camera" << std::endl;
        exit(1);
    }
    this->size = std::make_shared<cv::Size>(columns, rows);
    std::cout << "Photodiode of size " << rows << "x" << columns << " created" << std::endl;
    this->reset();
}

void Photodiode::reset() {
    if (this->capture == nullptr) {
        std::cerr << "No video capture defined" << std::endl;
    }
    this->capture->read(this->frame);
    if (this->frame.empty()) {
        std::cerr << "ERROR! blank frame grabbed" << std::endl;
    }

    cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2GRAY);
    int width = frame.cols;
    int height = frame.rows;
    cv::Mat cropFrame = this->frame(cv::Rect((width-height)/2, 0, height-1, height-1));
    cv::resize(cropFrame, cropFrame, *this->size);
    cropFrame.convertTo(this->frame, CV_32F);
}

void Photodiode::read(AnalogueRegister& reg) {
    this->frame.copyTo(reg.value());
}

Photodiode::~Photodiode() {
}



