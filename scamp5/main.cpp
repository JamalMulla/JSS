//
// Created by Jamal on 20/02/2021.
//

#include <simulator/external/instruction_parser.h>
#include <simulator/ui/ui.h>
#include <simulator/util/utility.h>

#include "scamp5.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(int argc, char **argv) {
    SCAMP5 scamp = SCAMP5::builder {}
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
    scamp.set_superpixel(bitorder, 4, 16);


    UI ui;
    ui.start();

    // One command which wraps up everything. pass it argc, argv, instance

    Instructions instructions = InstructionParser::parse(scamp, argc, argv);

    int frames = 1000;

    int i = 0;
    while(i < frames) {
        int e1 = cv::getTickCount();
        InstructionParser::execute(instructions, scamp);
        int e2 = cv::getTickCount();
//        multiple_sobel.txt(scamp);
//        scamp.histogram(scamp.A);
        std::cout << ((e2 - e1) / cv::getTickFrequency()) * 1000 << " ms" << std::endl;
        ui.display_reg(scamp.A);
        ui.display_reg(scamp.B);
        ui.display_reg(scamp.C);
        ui.display_reg(scamp.D);
        ui.display_reg(scamp.E);
        ui.display_reg(scamp.R5);
        ui.display_reg(scamp.R6);
        ui.display_reg(scamp.R7);
        if (i % 50 == 0) {
            std::cout << "Frame: " << i << "/" << frames << std::endl;
        }
        i++;
    }

    scamp.print_stats();

    return 0;
}


#pragma clang diagnostic pop
