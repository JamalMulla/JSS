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


    s.scamp5_load_pattern(s.R4,  0, 0, 123, 234);
    while(true) {

        s.get_image(s.C,s.D);
        s.sub(s.A, s.C, s.E);
        s.where(s.A);
        s.MOV(s.R5,s.FLAG);
        s.all();


        utility::display_register("R4", s.R4);
        utility::display_register("R5", s.R5);
        cv::waitKey(1);
    }

    return 0;
}
#pragma clang diagnostic pop
