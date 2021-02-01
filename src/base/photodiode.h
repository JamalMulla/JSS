//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PHOTODIODE_H
#define SIMULATOR_PHOTODIODE_H


#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include "component.h"

class Photodiode : public Component {
private:
    cv::VideoCapture *capture;
    cv::Size* size;
    cv::Mat frame;
    int rows_;
    int columns_;
public:
    Photodiode(int rows, int columns);
    void reset();
    [[nodiscard]] const cv::Mat& read() const;
    ~Photodiode();
};


#endif //SIMULATOR_PHOTODIODE_H
