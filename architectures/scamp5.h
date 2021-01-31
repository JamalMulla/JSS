//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_SCAMP5_H
#define SIMULATOR_SCAMP5_H


#include "../src/base/array.h"
#include <opencv2/opencv.hpp>

enum news_t { east = 1, west = 2, north = 4, south = 8, alldir = 15 };

typedef AnalogueRegister AREG;
typedef DigitalRegister DREG;

class SCAMP5 : public Array {
public:
    void nop();
    // Image Capturing
    void rpix();
    void get_image(AREG& y);
    void get_image(AREG& y, AREG& h);
    void respix();
    void respix(AREG& y);
    void getpix(AREG& y, AREG& pix_res);
    void getpix(AREG& y, AREG& h, AREG& pix_res);
};


#endif //SIMULATOR_SCAMP5_H
