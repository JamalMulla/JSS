//
// Created by Jamal on 20/02/2021.
//

#include "scamp5.h"

int main() {
    SCAMP5 s;

    while (true) {
        s.get_image(s.A, s.D);
        int e1 = cv::getTickCount();
        s.movx(s.B, s.A, south);
        s.add(s.B, s.B, s.A);
        s.movx(s.A, s.B, north);
        s.addx(s.B, s.B, s.A, east);
        s.sub2x(s.A, s.B, west, west, s.B);
        int e2 = cv::getTickCount();
        std::cout << ((e2 - e1)/cv::getTickFrequency()) * 1000 << " ms" << std::endl;
    }



    return 0;
}
