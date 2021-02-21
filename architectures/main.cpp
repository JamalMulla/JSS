//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <simulator/util/utility.h>
#include "instruction_factory.h"

int main() {
    SCAMP5 s;

    s.scamp5_in(s.E, 10);
    int i = 0;
    while(i < 250) {
//        s.get_image(s.C, s.D);
        InstructionFactory<SCAMP5>::execute(s, "get_image", s.C, s.D);
        s.sub(s.A, s.C, s.E);
        s.where(s.A);
        s.MOV(s.R5, s.FLAG);
        s.all();

        utility::display_register("PIX", s.PIX);
        utility::display_register("A", s.A);
        utility::display_register("B", s.B);
        utility::display_register("C", s.C);
        utility::display_register("D", s.D);
        utility::display_register("E", s.E);
        utility::display_register("FLAG", s.FLAG);
        utility::display_register("R5", s.R5);
        utility::display_register("NEWS", s.NEWS);
        cv::waitKey(1);
        i++;
        std::cout << "\rIteration " << i << std::endl;

    }

//    std::unordered_map<std::string, void (SCAMP5::*)()> instructions;
//    std::unordered_map<std::string, AnyCallable<void>()> instructions;


    return 0;
}
