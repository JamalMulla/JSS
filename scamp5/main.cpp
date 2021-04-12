//
// Created by Jamal on 20/02/2021.
//

#include <simulator/util/utility.h>

#include "scamp5.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void sobel(SCAMP5& s);
void superpixel(SCAMP5& s);

int main() {
    SCAMP5 s = SCAMP5::builder{}
                   .with_rows(256)
                   .with_cols(256)
                   .build();
    UI ui;
    ui.start();

    while(true) {
        int e1 = cv::getTickCount();
        sobel(s);
        int e2 = cv::getTickCount();
        std::cout << ((e2 - e1) / cv::getTickFrequency()) * 1000 << " ms" << std::endl;
        ui.display_reg(s.A);
        ui.display_reg(s.B);
        ui.display_reg(s.C);
        ui.display_reg(s.D);
        ui.display_reg(s.E);
        ui.display_reg(s.R5);
        ui.display_reg(s.R6);
        ui.display_reg(s.R7);
//        utility::display_register<int8_t>("ORIGINAL", *s.A);
//        utility::display_register<int8_t>("C", *s.C);
//        utility::display_register<int8_t>("SHIFT_RIGHT", *s.C);
//        cv::waitKey(1);
    }

    return 0;
}


inline void sobel(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.movx(s.B, s.A, south);
    s.add(s.B, s.B, s.A);
    s.movx(s.A, s.B, north);
    s.addx(s.B, s.B, s.A, east);
    s.sub2x(s.A, s.B, west, west, s.B);
}

inline void superpixel(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.superpixel_adc(s.R5, 0, s.A);
    s.superpixel_adc(s.R5, 1, s.D);
    s.superpixel_in(s.R6, 0, 30);
    s.superpixel_in(s.R7, 1, 30);
    s.superpixel_add(s.R5, 0, s.R5, s.R6);
    s.superpixel_sub(s.R5, 1, s.R5, s.R7);
//        s.superpixel_shift_left(s.R6, 0, s.R5);
    s.superpixel_dac(s.B, 0, s.R5);
    s.superpixel_dac(s.C, 1, s.R5);
}

#pragma clang diagnostic pop
