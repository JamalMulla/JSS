//
// Created by Jamal on 20/02/2021.
//

#include <simulator/external/parser.h>
#include <simulator/ui/ui.h>

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

    Parser::parse_config(config, program);

//    SCAMP5 scamp = SCAMP5::builder {}
//                   .with_rows(256)
//                   .with_cols(256)
//                   .build();
//
//    Bitorder bitorder = {
//        {{1, 8, 0, 0},
//         {2, 7, 0, 0},
//         {3, 6, 0, 0},
//         {4, 5, 0, 0}},
//        {{0, 0, 1, 8},
//         {0, 0, 2, 7},
//         {0, 0, 3, 6},
//         {0, 0, 4, 5}}};
//    Bitorder bitorder = {
//        {{1, 8, 9, 16},
//         {2, 7, 10, 15},
//         {3, 6, 11, 14},
//         {4, 5, 12, 13}}};
//    scamp.set_superpixel(bitorder, 4, 16);

    return 0;
}


#pragma clang diagnostic pop
