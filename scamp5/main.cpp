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
//        motion_detect(s);
        int e2 = cv::getTickCount();
//        multiple_sobel(s);
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

    static bool has_parsed = false;
    const std::string program = "/home/jm1417/Simulator/misc/sobel.txt";

    type scamp = type::get_by_name("SCAMP5");
    std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > parsed = InstructionParser::parse(scamp, rttr::instance(s), program);
    has_parsed = true;


    for(auto& instr: parsed) {
        method& method = instr.first;
        std::vector<rttr::variant>& args = instr.second;
        switch(args.size()) {
            case 0: method.invoke(s); break;
            case 1: method.invoke(s, args[0]); break;
            case 2: method.invoke(s, args[0], args[1]); break;
            case 3: method.invoke(s, args[0], args[1], args[2]); break;
            case 4: method.invoke(s, args[0], args[1], args[2], args[3]); break;
            case 5: method.invoke(s, args[0], args[1], args[2], args[3], args[4]); break;
            default: std::cerr << "Too many arguments in method " << method.get_name() << " invocation" << std::endl;
        }
    }


//    type class_type = type::get_by_name("SCAMP5");
//    property A = class_type.get_property("A");
//    property B = class_type.get_property("B");
//    property D = class_type.get_property("D");
//    method get_image = class_type.get_method("get_image");
//    method movx = class_type.get_method("movx");
//    method addx = class_type.get_method("addx");
//    method sub2x = class_type.get_method("sub2x");
//
//    get_image.invoke(s, A.get_value(s), D.get_value(s));
//    movx.invoke(s, B.get_value(s), A.get_value(s), south);
//    addx.invoke(s, A.get_value(s), B.get_value(s), A.get_value(s), north);
//    addx.invoke(s, B.get_value(s), B.get_value(s), A.get_value(s), east);
//    sub2x.invoke(s, A.get_value(s), B.get_value(s), west, west, B.get_value(s));
}

inline void vertical_sobel(SCAMP5& s) {
    s.get_image(s.A, s.D);
//    s.movx(s.B, s.A, south);
//    s.add(s.B, s.B, s.A);
//    s.movx(s.A, s.B, north);
//    s.addx(s.B, s.B, s.A, east);
//    s.sub2x(s.A, s.B, west, west, s.B);
    s.movx(s.B, s.A, south);
    s.addx(s.A, s.B, s.A, north);
    s.addx(s.B, s.B, s.A, east);
    s.sub2x(s.A, s.B, west, west, s.B);
}

inline void horizontal_sobel(SCAMP5& s) {
    s.get_image(s.A, s.D);
//    s.movx(s.B, s.A, west);
//    s.addx(s.A, s.B, s.A, east);
//    s.addx(s.B, s.B, s.A, south);
//    s.sub2x(s.A, s.B, north, north, s.B);
    s.movx(s.B, s.A, south);
    s.mov2x(s.D, s.A, west, north);
    s.sub(s.C, s.A, s.B);
    s.add(s.A, s.A, s.C);
    s.movx(s.B, s.A, west);
    s.sub2x(s.C, s.C, north, east, s.D);
    s.add(s.A, s.B, s.A, s.C);
}

inline void superpixel_sobel(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.superpixel_adc(s.R5, 0, s.A);
    s.superpixel_movx(s.R6, s.R5, south);
    s.superpixel_movx(s.R7, s.R6, north);
    s.superpixel_movx(s.R7, s.R7, north);
    s.superpixel_sub(s.R5, 0, s.R7, s.R6);
    s.superpixel_dac(s.B, 0, s.R5);
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

inline void multiple_sobel(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.neg(s.C, s.A);
    s.subx(s.B, s.A, south, s.C);
    s.subx(s.D, s.A, north, s.C);
    s.subx(s.C, s.C, west, s.A);
    s.movx(s.A, s.C, east);
    s.addx(s.C, s.C, s.A, north);
    s.addx(s.B, s.B, s.D, east);
    s.sub2x(s.A, s.B, west, west, s.B);
    s.sub2x(s.B, s.C, south, south, s.C);
}

inline void gaussian3x3(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.divq(s.C, s.A);
    s.divq(s.A, s.C);
    s.divq(s.C, s.A);
    s.divq(s.A, s.C);
    s.movx(s.C, s.A, south);
    s.add(s.A, s.C, s.A);
    s.movx(s.C, s.A, north);
    s.add(s.A, s.A, s.C);
    s.movx(s.C, s.A, west);
    s.add(s.A, s.C, s.A);
    s.movx(s.C, s.A, east);
    s.add(s.A, s.A, s.C);
}

inline void gaussian5x5(SCAMP5& s) {
    s.get_image(s.A, s.D);
    s.div(s.E, s.C, s.A);
    s.diva(s.E, s.C, s.B);
    s.diva(s.E, s.C, s.B);
    s.diva(s.E, s.C, s.B);
    s.diva(s.E, s.C, s.B);
    s.neg(s.C, s.E);
    s.neg(s.B, s.C);
    s.diva(s.B, s.C, s.F);
    s.movx(s.C, s.B, south);
    s.movx(s.F, s.B, north);
    s.add(s.D, s.B, s.E);
    s.add(s.B, s.B, s.E, s.D);
    s.addx(s.D, s.E, s.D, north);
    s.add(s.C, s.C, s.E, s.F);
    s.mov2x(s.A, s.C, west, north);
    s.add2x(s.F, s.E, s.F, north, north);
    s.add2x(s.D, s.E, s.D, south, south);
    s.add2x(s.E, s.D, s.E, north, north);
    s.add(s.A, s.F, s.C, s.A);
    s.add2x(s.A, s.A, s.E, west, south);
    s.add(s.B, s.B, s.D, s.E);
    s.add2x(s.C, s.C, s.A, east, east);
    s.add(s.A, s.B, s.A, s.C);
}

inline void motion_detect(SCAMP5& s) {
    s.get_image(s.C,s.D);
    s.sub(s.D,s.C,s.F);	// D = C - F
    s.mov(s.F,s.C);	// F = C
    s.abs(s.B,s.D);	// B = |D|
    s.sub(s.A,s.B,s.E);	// A = B - E
    s.where(s.A);	// where A > 0
        s.MOV(s.R5, s.FLAG);
    s.all();
}

#pragma clang diagnostic pop
