//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PHOTODIODE_H
#define SIMULATOR_PHOTODIODE_H


#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include "../registers/analogue_register.h"

class Photodiode {
private:
    cv::VideoCapture *capture;
    cv::Size *size;
    cv::Mat frame;
    int rows_;
    int columns_;
public:
    Photodiode();
    Photodiode(int rows, int columns);
    void reset();
    void read(AnalogueRegister& reg);
    ~Photodiode();
};


#endif //SIMULATOR_PHOTODIODE_H
