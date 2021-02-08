#include "src/base/array.h"
#include "src/base/processing_element.h"
#include "architectures/scamp5.h"
#include "src/utility.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define START_TIMER() auto TIME_START = std::chrono::high_resolution_clock::now()
#define END_TIMER() auto TIME_END = std::chrono::high_resolution_clock::now(); std::cout << "Elapsed time: " \
<< std::chrono::duration_cast<std::chrono::microseconds>(TIME_END-TIME_START).count() << " microseconds\n"

int main() {
    SCAMP5 s;

    int i = 0;
    while(i < 2000) {

//        s.E.value().setTo(0);
//        s.get_image(s.C,s.D);
//        s.sub(s.A, s.C, s.E);
//        s.where(s.A);
//        s.MOV(s.R5,s.FLAG);
//        s.all();


//        Sobel
//        s.get_image(s.A, s.D);
//        s.movx(s.B, s.A, south);
//        s.add(s.B, s.B, s.A);
//        s.movx(s.A, s.B, north);
//        s.addx(s.B, s.B, s.A, east);
//        s.sub2x(s.A, s.B, west, west, s.B);

        // Multiple Sobel
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


        utility::display_register("PIX", s.PIX);
        utility::display_register("A", s.A);
        utility::display_register("B", s.B);
        utility::display_register("FLAG", s.FLAG);
        utility::display_register("R5", s.R5);
        utility::display_register("NEWS", s.NEWS);
        cv::waitKey(1);
        i++;
        std::cout << "\rIteration " << i << std::flush;
    }

    std::cout << std::endl;

    s.print_stats();

//    std::cout << "FLAG reads: " << s.FLAG.get_reads() << " FLAG writes: " << s.FLAG.get_writes() << std::endl;
//    std::cout << "R5 reads: " << s.R5.get_reads() << " R5 writes: " << s.R5.get_writes() << std::endl;
//
//    //in uwatts
//    std::cout << "Power for FLAG reads: " << ((s.FLAG.get_reads() * 0.5655) / (1e+7)) / 10 << "w" << std::endl;

    return 0;
}
#pragma clang diagnostic pop