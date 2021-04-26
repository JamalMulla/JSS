//
// Created by Jamal on 20/02/2021.
//

#include <simulator/external/parser.h>
#include <simulator/ui/ui.h>
#include <simulator/util/utility.h>

#include <fstream>

#include "scamp5.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

int main(int argc, char **argv) {

    if (argc < 3) {
        std::cerr << "[Error] Must provide a config file and a program file" << std::endl;
        exit(EXIT_FAILURE);
    }
    // config first then program file
    std::string config_path = argv[1];
    std::string program_path = argv[2];

    std::ifstream config(config_path);
    if (!config.is_open()) {
        std::cerr << "Error opening config file " << config_path << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream program(program_path);
    if (!program.is_open()) {
        std::cerr << "Error opening program file " << program_path << std::endl;
        exit(EXIT_FAILURE);
    }

    Parser::parse_config(config);

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

    Instructions instructions = Parser::parse_instructions(scamp, program);

    UI ui;
    ui.start();

    int frames = 1000;

    int i = 0;
    while(i < frames) {
        int e1 = cv::getTickCount();
        Parser::execute_instructions(instructions, scamp);
        int e2 = cv::getTickCount();
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
