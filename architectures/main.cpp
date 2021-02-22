//
// Created by Jamal on 20/02/2021.
//

#include <iostream>
#include "scamp5.h"
#include <simulator/util/utility.h>

#define START_TIMER() auto TIME_START = std::chrono::high_resolution_clock::now()
#define END_TIMER() auto TIME_END = std::chrono::high_resolution_clock::now(); std::cout << "Elapsed time: " \
<< std::chrono::duration_cast<std::chrono::milliseconds>(TIME_END-TIME_START).count() << " ms\n"

int main(int argc, char** argv) {

    if (argc == 1) {
        std::cerr << "[Error] No program file given" << std::endl;
        exit(1);
    }

    SCAMP5 s;

    s.scamp5_in(s.E, 10);
    int i = 0;

    InstructionParser<SCAMP5> parser(s.registers);
    parser.parse(s, std::string(argv[1]), 0);

    // Assume we've parsed this and got the following
//    const char *func_name = "get_image";
//    std::string arg1 = "C";
//    std::string arg2 =  "D";
//
//    auto& a1 = s.C;
//    auto& a2 = s.D;
//    auto f = InstructionFactory<SCAMP5>::get_instruction<AREG, AREG>(func_name);
//    (s.*f)(a1, a2);
//    InstructionFactory<SCAMP5>::execute(s, func_name, a1, a2);
//
//    // Need to get the function using the type arguments. So need type arguments first
//    AREG* C;
//    AREG* D;
//    try
//    {
//        C = std::any_cast<AREG*>(s.registers.get_arg("C"));
//        D = std::any_cast<AREG*>(s.registers.get_arg("D"));
//    }
//    catch (const std::bad_any_cast& e)
//    {
//        std::cout << e.what() << '\n';
//    }


//
//
//    // Get function
//
//    auto get_image = InstructionFactory<SCAMP5>::get_instruction<AREG, AREG>("get_image");
//
//    while(i < 250) {
//        START_TIMER();
////        (s.*get_image)(s.C, s.D);
//        InstructionFactory<SCAMP5>::execute(s, "get_image", *C, *D);
//        END_TIMER();
//        s.get_image(s.C, s.D);
//
//        s.sub(s.A, s.C, s.E);
//        s.where(s.A);
//        s.MOV(s.R5, s.FLAG);
//        s.all();
//
//        utility::display_register("PIX", s.PIX);
//        utility::display_register("A", s.A);
//        utility::display_register("B", s.B);
//        utility::display_register("C", s.C);
//        utility::display_register("D", s.D);
//        utility::display_register("E", s.E);
//        utility::display_register("FLAG", s.FLAG);
//        utility::display_register("R5", s.R5);
//        utility::display_register("NEWS", s.NEWS);
//        cv::waitKey(1);
//        i++;
//        std::cout << "\rIteration " << i << std::endl;
//
//    }

//    std::unordered_map<std::string, void (SCAMP5::*)()> instructions;
//    std::unordered_map<std::string, AnyCallable<void>()> instructions;


    return 0;
}
