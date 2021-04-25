//
// Created by Jamal on 20/02/2021.
//

#include <simulator/external/instruction_parser.h>
#include <simulator/ui/ui.h>
#include <simulator/util/utility.h>

#include "scamp5.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

void vertical_sobel(SCAMP5& s);
void horizontal_sobel(SCAMP5& s);
void superpixel_sobel(SCAMP5& s);
void superpixel(SCAMP5& s);
void multiple_sobel(SCAMP5& s);
void gaussian3x3(SCAMP5& s);
void gaussian5x5(SCAMP5& s);
void motion_detect(SCAMP5& s);
void rttr_test(SCAMP5& s);

int main() {
    SCAMP5 s = SCAMP5::builder {}
                   .with_rows(256)
                   .with_cols(256)
                   .build();
//    Bitorder bitorder = {
//        {{1, 8, 0, 0},
//         {2, 7, 0, 0},
//         {3, 6, 0, 0},
//         {4, 5, 0, 0}},
//        {{0, 0, 1, 8},
//         {0, 0, 2, 7},
//         {0, 0, 3, 6},
//         {0, 0, 4, 5}}};
    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}}};
    s.set_superpixel(bitorder, 4, 16);


    UI ui;
    ui.start();

    int frames = 1000;

    int i = 0;
    while(i < frames) {
        int e1 = cv::getTickCount();
        rttr_test(s);
        int e2 = cv::getTickCount();
//        multiple_sobel.txt(s);
//        s.histogram(s.A);
        std::cout << ((e2 - e1) / cv::getTickFrequency()) * 1000 << " ms" << std::endl;
        ui.display_reg(s.A);
        ui.display_reg(s.B);
        ui.display_reg(s.C);
        ui.display_reg(s.D);
        ui.display_reg(s.E);
        ui.display_reg(s.R5);
        ui.display_reg(s.R6);
        ui.display_reg(s.R7);
        if (i % 50 == 0) {
            std::cout << "Frame: " << i << "/" << frames << std::endl;
        }
        i++;
    }

    s.print_stats();

    return 0;
}

using namespace rttr;

inline void rttr_test(SCAMP5& s) {

    const std::string program = "/home/jm1417/Simulator/scamp5/examples/multiple_sobel.txt";

    type scamp = type::get_by_name("SCAMP5");
    std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > parsed = InstructionParser::parse(scamp, rttr::instance(s), program);


    for(auto& instr: parsed) {
        method& method = instr.first;
        std::vector<rttr::variant>& args = instr.second;
        variant res;
        switch(args.size()) {
            case 0: res = method.invoke(s); break;
            case 1: res = method.invoke(s, args[0]); break;
            case 2: res = method.invoke(s, args[0], args[1]); break;
            case 3: res = method.invoke(s, args[0], args[1], args[2]); break;
            case 4: res = method.invoke(s, args[0], args[1], args[2], args[3]); break;
            case 5: res = method.invoke(s, args[0], args[1], args[2], args[3], args[4]); break;
            default: std::cerr << "Too many arguments in method " << method.get_name() << " invocation" << std::endl;
        }
        if (!res.is_valid()) {
            std::cerr << "Could not execute method \"" << method.get_name() << "\"" << " with " << args.size() << " arguments" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}


inline void superpixel(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.superpixel_adc(s.R5, 0, s.A);
//    s.superpixel_adc(s.R5, 1, s.D);
    s.superpixel_in(s.R6, 0, 30);
//    s.superpixel_in(s.R7, 1, 30);
    s.superpixel_add(s.R5, 0, s.R5, s.R6);
    s.superpixel_sub(s.R5, 0, s.R5, s.R6);
//    s.superpixel_sub(s.R5, 1, s.R5, s.R7);
//    s.superpixel_shift_left(s.R7, 1, s.R5);
    s.superpixel_dac(s.B, 0, s.R5);
//    s.superpixel_dac(s.C, 1, s.R5);
}



#pragma clang diagnostic pop
