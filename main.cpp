#include "src/base/array.h"
#include "src/base/processing_element.h"
#include "architectures/scamp5.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#define START_TIMER() auto TIME_START = std::chrono::high_resolution_clock::now()
#define END_TIMER() auto TIME_END = std::chrono::high_resolution_clock::now(); std::cout << "Elapsed time: " \
<< std::chrono::duration_cast<std::chrono::microseconds>(TIME_END-TIME_START).count() << " microseconds\n"


cv::Mat draw_analogue_register(cv::Mat& reg, const std::string& window){
    double minVal, maxVal;
    cv::minMaxLoc(reg, &minVal, &maxVal);
    cv::Mat draw;
    reg.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
    return draw;
}

int main() {
    SCAMP5 s;

    while(true) {

        s.get_image(s.C,s.D);
        s.sub(s.A, s.C, s.E);
        s.where(s.A);
        s.MOV(s.R5,s.FLAG);
        s.all();


//        s.get_image(s.A, s.D);
//        s.movx(s.B, s.A, south);
//        s.add(s.B, s.B, s.A);
//        s.movx(s.A, s.B, north);
//        s.addx(s.B, s.B, s.A, east);
//        s.sub2x(s.A, s.B, west, west, s.B);


        cv::imshow("PIX", draw_analogue_register(s.PIX.value(), "PIX"));
        cv::imshow("A", draw_analogue_register(s.A.value(), "A"));
        cv::imshow("B", draw_analogue_register(s.B.value(), "B"));
        cv::imshow("R5", draw_analogue_register(s.R5.value(), "R5"));
        cv::imshow("NEWS", draw_analogue_register(s.NEWS.value(), "NEWS"));
        cv::waitKey(1);
    }


    return 0;
}
#pragma clang diagnostic pop