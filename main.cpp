#include "src/base/array.h"
#include "src/base/processing_element.h"
#include "architectures/scamp5.h"
#include "src/util/utility.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define START_TIMER() auto TIME_START = std::chrono::high_resolution_clock::now()
#define END_TIMER() auto TIME_END = std::chrono::high_resolution_clock::now(); std::cout << "Elapsed time: " \
<< std::chrono::duration_cast<std::chrono::milliseconds>(TIME_END-TIME_START).count() << " ms\n"

int main() {
    SCAMP5 s;

    int i = 0;
    s.scamp5_in(s.E, 10);
    while(i < 250) {
        s.get_image(s.C,s.D);
        s.sub(s.A, s.C, s.E);
        s.where(s.A);
        s.MOV(s.R5,s.FLAG);
        s.all();


//        Sobel
//        s.get_image(s.A, s.D);
//        s.movx(s.B, s.A, south);
//        s.add(s.B, s.B, s.A);
//        s.movx(s.A, s.B, north);
//        s.addx(s.B, s.B, s.A, east);
//        s.sub2x(s.A, s.B, west, west, s.B);

        // Multiple Sobel
//        START_TIMER();
//        s.get_image(s.A, s.D);
////        END_TIMER();
//        s.neg(s.C, s.A);
//        s.subx(s.B, s.A, south, s.C);
//        s.subx(s.D, s.A, north, s.C);
//        s.subx(s.C, s.C, west, s.A);
//        s.movx(s.A, s.C, east);
//        s.addx(s.C, s.C, s.A, north);
//        s.addx(s.B, s.B, s.D, east);
//        s.sub2x(s.A, s.B, west, west, s.B);
//        s.sub2x(s.B, s.C, south, south, s.C);

        // Gaussian 5x5
//        s.get_image(s.A, s.D);
//        s.div(s.E, s.C, s.A);
//        s.diva(s.E, s.C, s.B);
//        s.diva(s.E, s.C, s.B);
//        s.diva(s.E, s.C, s.B);
//        s.diva(s.E, s.C, s.B);
//        s.neg(s.C, s.E);
//        s.neg(s.B, s.C);
//        s.diva(s.B, s.C, s.F);
//        s.movx(s.C, s.B, south);
//        s.movx(s.F, s.B, north);
//        s.add(s.D, s.B, s.E);
//        s.add(s.B, s.B, s.E, s.D);
//        s.addx(s.D, s.E, s.D, north);
//        s.add(s.C, s.C, s.E, s.F);
//        s.mov2x(s.A, s.C, west, north);
//        s.add2x(s.F, s.E, s.F, north, north);
//        s.add2x(s.D, s.E, s.D, south, south);
//        s.add2x(s.E, s.D, s.E, north, north);
//        s.add(s.A, s.F, s.C, s.A);
//        s.add2x(s.A, s.A, s.E, west, south);
//        s.add(s.B, s.B, s.D, s.E);
//        s.add2x(s.C, s.C, s.A, east, east);
//        s.add(s.A, s.B, s.A, s.C);

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

    std::cout << std::endl;

    s.print_stats(s.cycles);

    return 0;
}
#pragma clang diagnostic pop