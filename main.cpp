//
// Created by Jamal on 20/02/2021.
//

#include <simulator/external/parser.h>
#include <fstream>
#include "scamp5/analognet2/analog_main.h"

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

    Parser::get_instance().parse_config(config, program);
    return 0;
}
