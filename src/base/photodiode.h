//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PHOTODIODE_H
#define SIMULATOR_PHOTODIODE_H


#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include "../registers/analogue_register.h"
#include "component.h"

class Photodiode : public Component {
private:
    std::shared_ptr<cv::VideoCapture> capture;
    std::shared_ptr<cv::Size> size;
    cv::Mat frame;
    int rows_;
    int columns_;
public:
    Photodiode(int rows, int columns);
    void reset();
    void read(AnalogueRegister& reg);
    void print_stats(CycleCounter counter) override;
    ~Photodiode();
};


#endif //SIMULATOR_PHOTODIODE_H
