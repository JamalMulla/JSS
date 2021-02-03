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
    SCAMP5 scamp5;

    while(true) {
        scamp5.get_image(scamp5.A, scamp5.D);
        scamp5.movx(scamp5.B, scamp5.A, south);
        scamp5.add(scamp5.B, scamp5.B, scamp5.A);
        scamp5.movx(scamp5.A, scamp5.B, north);
        scamp5.addx(scamp5.B, scamp5.B, scamp5.A, east);
        scamp5.sub2x(scamp5.A, scamp5.B, west, west, scamp5.B);

        cv::imshow("PIX", draw_analogue_register(scamp5.PIX.value(), "PIX"));
        cv::imshow("A", draw_analogue_register(scamp5.A.value(), "A"));
        cv::imshow("B", draw_analogue_register(scamp5.B.value(), "B"));
        cv::imshow("NEWS", draw_analogue_register(scamp5.NEWS.value(), "NEWS"));
        cv::waitKey(1);
    }


    return 0;
}
#pragma clang diagnostic pop