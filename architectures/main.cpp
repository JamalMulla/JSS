//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <simulator/util/utility.h>
#include <opencv2/core.hpp>

#define START_TIMER() auto TIME_START = std::chrono::high_resolution_clock::now()
#define END_TIMER() auto TIME_END = std::chrono::high_resolution_clock::now(); std::cout << "Elapsed time: " \
<< std::chrono::duration_cast<std::chrono::milliseconds>(TIME_END-TIME_START).count() << " ms\n"

int main(int argc, char** argv) {

    int iterations = -1;

    if (argc == 1) {
        std::cerr << "[Error] No program file given" << std::endl;
        exit(1);
    } else if (argc == 3) {
        std::string arg = argv[2];
        try {
            std::size_t pos;
            iterations = std::stoi(arg, &pos);
            if (pos < arg.size()) {
                std::cerr << "Trailing characters after number: " << arg << '\n';
            }
        } catch (std::invalid_argument const &ex) {
            std::cerr << "Invalid number: " << arg << '\n';
        } catch (std::out_of_range const &ex) {
            std::cerr << "Number out of range: " << arg << '\n';
        }
    }


    SCAMP5 s;

    InstructionParser<SCAMP5> parser(s.registers);
    parser.parse(s, std::string(argv[1]), iterations);

    return 0;
}
