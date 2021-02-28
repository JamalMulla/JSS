//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <simulator/util/utility.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main() {

    SCAMP5 s;

    int i = 0;
    s.scamp5_in(s.E, 1);


//    s.scamp5_load_pattern(s.R4,  0, 0, 123, 234);


    while (true) {
        s.scamp5_get_image(s.A, s.B, 1);
//        s.scamp5_draw_begin(s.R2);
//        s.scamp5_draw_circle(127, 127, 10);
//        s.scamp5_draw_line(127, 32, 290, 32);
//        s.scamp5_draw_negate();
//        s.scamp5_draw_end();

        // Threshold
//        s.get_image(s.C,s.D);
//        s.sub(s.A, s.C, s.E);
//        s.where(s.A);
//        s.MOV(s.R5,s.FLAG);
//        s.all();


//       Multiple Sobel
//        s.get_image(s.A, s.D);
//        int before = cv::getTickCount();
//        s.neg(s.C, s.A);
//        s.subx(s.B, s.A, south, s.C);
//        s.subx(s.D, s.A, north, s.C);
//        s.subx(s.C, s.C, west, s.A);
//        s.movx(s.A, s.C, east);
//        s.addx(s.C, s.C, s.A, north);
//        s.addx(s.B, s.B, s.D, east);
//        s.sub2x(s.A, s.B, west, west, s.B);
//        s.sub2x(s.B, s.C, south, south, s.C);
//        int after = cv::getTickCount();
//        std::cout << "Ticks: " << after - before << std::endl;


//        Motion detection
//        s.get_image(s.C, s.D);
//        s.sub(s.D,s.C,s.F);	// D = C - F
//        s.mov(s.F,s.C);	// F = C
//        s.abs(s.B,s.D);	// B = |D|
//        s.sub(s.A,s.B,s.E);	// A = B - E
//        s.where(s.A);	// where A > 0
//        s.MOV(s.R5,s.FLAG);
//        s.all();


        utility::display_register("R4", s.R4);
        utility::display_register("C", s.C);
        utility::display_register("A", s.A);
        utility::display_register("B", s.B);
        utility::display_register("R5", s.R5);
        cv::waitKey(1);
    }

    return 0;
}

#pragma clang diagnostic pop
