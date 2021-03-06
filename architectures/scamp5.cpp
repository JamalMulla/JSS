//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"
#include "simulator/memory/sram_6t.h"
#include "simulator/metrics/stats.h"
#include <filesystem>
#include <fstream>
#include <ostream>
#include <iostream>
#include <simulator/util/utility.h>

SCAMP5::SCAMP5() {
    // Initially all PEs are active
    this->FLAG.write(1);
    stats::set_clock_rate(1e7);
    this->FLAG.change_memory_type(SRAM_6T());
}

void SCAMP5::nop() {
    cycles++;
}

void SCAMP5::rpix() {
    //reset PIX
    this->pe.photodiode.reset();
    cycles++;
}

void SCAMP5::get_image(AREG &y) {
    //y := half-range image, and reset PIX
    this->pe.photodiode.read(PIX);
    double seconds = this->pe.photodiode.last_frame_time();
    int c = cycles.to_cycles(seconds, stats::CLOCK_RATE);
    cycles+=c/300;
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void SCAMP5::get_image(AREG &y, AREG &h) {
    //y := full-range image, h := negative half-range image, and reset PIX
    this->pe.photodiode.read(PIX);
    double seconds = this->pe.photodiode.last_frame_time();
    int c = cycles.to_cycles(seconds, stats::CLOCK_RATE);
    cycles+=c/300;
    this->bus(NEWS, PIX);
    this->bus(h, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, h, NEWS, PIX);

}

void SCAMP5::respix() {
    //reset PIX
    this->rpix();
    this->rpix();
    this->nop();
}

void SCAMP5::respix(AREG &y) {
    //reset PIX, keep its reset level in y
    this->rpix();
    this->rpix();
    this->nop();
    this->pe.photodiode.read(PIX);
    double seconds = this->pe.photodiode.last_frame_time();
    int c = cycles.to_cycles(seconds, stats::CLOCK_RATE);
    cycles+=c/300;
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void SCAMP5::getpix(AREG &y, AREG &pix_res) {
    //y := half-range image, supplying the reset level of PIX
    this->pe.photodiode.read(PIX);
    double seconds = this->pe.photodiode.last_frame_time();
    int c = cycles.to_cycles(seconds, stats::CLOCK_RATE);
    cycles+=c/300;
    this->bus(NEWS,PIX);
    this->bus(y,NEWS, pix_res);
}

void SCAMP5::getpix(AREG &y, AREG &h, AREG &pix_res) {
    //y := full-range, h := half-range image, supplying the reset level of PIX
    this->pe.photodiode.read(PIX);
    double seconds = this->pe.photodiode.last_frame_time();
    int c = cycles.to_cycles(seconds, stats::CLOCK_RATE);
    cycles+=c/300;
    this->bus(h,PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

void SCAMP5::bus(AREG &a) {
    // a = 0 + error
    this->pe.analogue_bus.bus(a, FLAG);
    cycles++;
}

void SCAMP5::bus(AREG &a, AREG &a0) {
    // a = -a0 + error
    this->pe.analogue_bus.bus(a, a0, FLAG);
    cycles++;
}

void SCAMP5::bus(AREG &a, AREG &a0, AREG &a1) {
    //a = -(a0 + a1) + error
    this->pe.analogue_bus.bus(a, a0, a1,FLAG);
    cycles+=4; // 2 reads, 1 add, 1 write
}

void SCAMP5::bus(AREG &a, AREG &a0, AREG &a1, AREG &a2) {
    //a = -(a0 + a1 + a2) + error
    this->pe.analogue_bus.bus(a, a0, a1, a2,FLAG);
    cycles+=5;  // 3 reads, 1 add, 1 write
}

void SCAMP5::bus(AREG &a, AREG &a0, AREG &a1, AREG &a2, AREG &a3) {
    //a = -(a0 + a1 + a2 + a3) + error
    this->pe.analogue_bus.bus(a, a0, a1, a2,a3, FLAG);
    cycles+=6;  // 4 reads, 1 add, 1 write
}

void SCAMP5::bus2(AREG &a, AREG &b) {
    //a,b = 0 + error
    this->pe.analogue_bus.bus2(a, b, FLAG);
    cycles+=2;  // 2 writes
}

void SCAMP5::bus2(AREG &a, AREG &b, AREG &a0) {
    //a,b = -0.5*a0 + error + noise
    this->pe.analogue_bus.bus2(a, b, a0,FLAG);
    cycles+=3;  // 1 read, 2 writes
}

void SCAMP5::bus2(AREG &a, AREG &b, AREG &a0, AREG &a1) {
    //a,b = -0.5*(a0 + a1) + error + noise
    this->pe.analogue_bus.bus2(a, b, a0, a1,FLAG);
    cycles+=5;  // 2 reads, 1 add, 2 writes
}

void SCAMP5::bus3(AREG &a, AREG &b, AREG &c, AREG &a0) {
    //a,b,c = -0.33*a0 + error + noise
    this->pe.analogue_bus.bus3(a, b, c, a0, FLAG);
    cycles+=2;  // 1 read, 3 writes
}

void SCAMP5::where(AREG &a) {
    //FLAG := a > 0.
    this->pe.analogue_bus.conditional_positive_set(a, FLAG);
    cycles+=2; // 1 read, 1 write
}

void SCAMP5::where(AREG &a0, AREG &a1) {
    //FLAG := (a0 + a1) > 0.
    this->pe.analogue_bus.conditional_positive_set(a0, a1, FLAG);
    cycles+=4;  // 2 reads, 1 add, 1 write
}

void SCAMP5::where(AREG &a0, AREG &a1, AREG &a2) {
    //FLAG := (a0 + a1 + a2) > 0.
    this->pe.analogue_bus.conditional_positive_set(a0, a1, a2, FLAG);
    cycles+=5;  // 3 reads, 1 add, 1 write
}

void SCAMP5::all() {
    //FLAG := 1.
    this->FLAG.set();
    cycles+=1;  // 1 writes
}

void SCAMP5::mov(AREG &y, AREG &x0) {
    //y = x0
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

void SCAMP5::res(AREG &a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void SCAMP5::res(AREG &a, AREG &b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void SCAMP5::add(AREG &y, AREG &x0, AREG &x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void SCAMP5::add(AREG &y, AREG &x0, AREG &x1, AREG &x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void SCAMP5::sub(AREG &y, AREG &x0, AREG &x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void SCAMP5::neg(AREG &y, AREG &x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void SCAMP5::abs(AREG &y, AREG &x0) {
    // y = |x0|
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void SCAMP5::div(AREG &y0, AREG &y1, AREG &y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::div(AREG &y0, AREG &y1, AREG &y2, AREG &x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2,NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::diva(AREG &y0, AREG &y1, AREG &y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void SCAMP5::divq(AREG &y0, AREG &x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

void SCAMP5::movx(AREG &y, AREG &x0, news_t dir) {
    // y = x0_dir
    AREG intermediate(y.value().rows, y.value().cols);
    this->bus(intermediate, x0);
    switch (dir) {
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS);
}

void SCAMP5::mov2x(AREG &y, AREG &x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir (note: this only works when FLAG is "all")
    AREG intermediate(y.value().rows, y.value().cols);
    AREG intermediate2(y.value().rows, y.value().cols);
    this->bus(intermediate, x0);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.push_north(NEWS, intermediate2, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_east(NEWS, intermediate2, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_west(NEWS, intermediate2, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_south(NEWS, intermediate2, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }

    this->bus(y, intermediate2);
}

void SCAMP5::addx(AREG &y, AREG &x0, AREG &x1, news_t dir) {
    // y := x0_dir + x1_dir
    AREG intermediate(y.value().rows, y.value().cols);
    this->bus(intermediate, x0, x1);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }

    this->bus(y, NEWS);
}

void SCAMP5::add2x(AREG &y, AREG &x0, AREG &x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    AREG intermediate(y.value().rows, y.value().cols);
    AREG intermediate2(y.value().rows, y.value().cols);
    this->bus(intermediate, x0, x1);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.pull_north(NEWS, intermediate2, 1, FLAG); break;
        case east: this->pe.analogue_bus.pull_east(NEWS, intermediate2, 1, FLAG); break;
        case west: this->pe.analogue_bus.pull_west(NEWS, intermediate2, 1, FLAG); break;
        case south: this->pe.analogue_bus.pull_south(NEWS, intermediate2, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, intermediate2);
}

void SCAMP5::subx(AREG &y, AREG &x0, news_t dir, AREG &x1) {
    // y := x0_dir - x1
    AREG intermediate(y.value().rows, y.value().cols);
    this->bus(intermediate, x0);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS, x1);
}

void SCAMP5::sub2x(AREG &y, AREG &x0, news_t dir, news_t dir2, AREG &x1) {
    // y := x0_dir_dir2 - x1
    AREG intermediate(y.value().rows, y.value().cols);
    AREG intermediate2(y.value().rows, y.value().cols);
    this->bus(intermediate, x0);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.pull_north(NEWS, intermediate2, 1, FLAG); break;
        case east: this->pe.analogue_bus.pull_east(NEWS, intermediate2, 1, FLAG); break;
        case west: this->pe.analogue_bus.pull_west(NEWS, intermediate2, 1, FLAG); break;
        case south: this->pe.analogue_bus.pull_south(NEWS, intermediate2, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, intermediate2, x1);
}

void SCAMP5::blurset() {
    // need to be used once ahead of a blur instruction
    // TODO check
    R1.set();
    R2.set();
}

void SCAMP5::blur(AREG &a, AREG &a0) {
    // blur a0 into a
    cv::GaussianBlur(a0.value(), a.value(), cv::Size(3, 3), 0);
    cv::bitwise_not(a.value(), a.value());
}

void SCAMP5::blurh(AREG &a, AREG &a0) {
    // analog asynchronized blur, horizontal only
    // row vector for x direction which is horizontal?
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat horizontal_kernel = cv::Mat(1, 6, CV_32F, gaussian_1d);

    cv::filter2D(a0.value(), a.value(), -1, horizontal_kernel);
    cv::bitwise_not(a.value(), a.value());

}

void SCAMP5::blurv(AREG &a, AREG &a0) {
    // analog asynchronized blur, vertical only
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat vertical_kernel = cv::Mat(6, 1, CV_32F, gaussian_1d);

    cv::filter2D(a0.value(), a.value(), -1, vertical_kernel);
    cv::bitwise_not(a.value(), a.value());
}

void SCAMP5::gauss(AREG &y, AREG &x, const int iterations) {
    // blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly
    blur(NEWS, x);
    blur(y, NEWS);
    for (int i = 1; i < iterations; i++) {
        blur(NEWS, y);
        blur(y, NEWS);
    }
}

void SCAMP5::gaussh(AREG &y, AREG &x, const int iterations) {
    // horizontally blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly
    blurh(NEWS, x);
    blurh(y, NEWS);
    for (int i = 1; i < iterations; i++) {
        blurh(NEWS, y);
        blurh(y, NEWS);
    }
}

void SCAMP5::gaussv(AREG &y, AREG &x, const int iterations) {
    // vertically blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly
    blurv(NEWS, x);
    blurv(y, NEWS);
    for (int i = 1; i < iterations; i++) {
        blurv(NEWS, y);
        blurv(y, NEWS);
    }
}

void SCAMP5::newsblur(AREG &y, AREG &x, const int iterations) {
    // blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
    float data[3][3] = {{0,1,0},{1,0,1},{0,1,0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
    cv::bitwise_not(y.value(), y.value());

    for (int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
        cv::bitwise_not(y.value(), y.value());
    }
}

void SCAMP5::newsblurh(AREG &y, AREG &x, const int iterations) {
    // horizontally blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
    float data[3][3] = {{0,0,0},{1,0,1},{0,0,0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
    cv::bitwise_not(y.value(), y.value());

    for (int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
        cv::bitwise_not(y.value(), y.value());
    }
}

void SCAMP5::newsblurv(AREG &y, AREG &x, const int iterations) {
    // vertically blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
    float data[3][3] = {{0,1,0},{0,0,0},{0,1,0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
    cv::bitwise_not(y.value(), y.value());

    for (int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS.value(), y.value(), -1, neighbour_kernel);
        cv::bitwise_not(y.value(), y.value());
    }
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1) {
    // d := d0 OR d1
    DigitalBus::OR(d, d0);
    cycles+=4;  // 2 reads, 1 or, 1 write
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2) {
    // d := d0 OR d1 OR d2
    DigitalBus::OR(d, d0, d1, d2);
    cycles+=5;  // 3 reads, 1 or, 1 write
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d := d0 OR d1 OR d2 OR d3
    DigitalBus::OR(d, d0, d1, d2, d3);
    cycles+=6;  // 4 reads, 1 or, 1 write
}

void SCAMP5::NOT(DREG &d, DREG &d0) {
    // d := NOT d0
    DigitalBus::NOT(d, d0);
    cycles+=3;  // 1 read, 1 op, 1 write

}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1) {
    // d := NOR(d0 OR d1)
    DigitalBus::NOR(d, d0, d1);
    cycles+=5;  // 2 reads, 2 op, 1 write
}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2) {
    // d := NOR(d0 OR d1 OR d2)
    DigitalBus::NOR(d, d0, d1, d2);
    cycles+=6;  // 3 reads, 2 op, 1 write
}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d := NOTRd0 OR d1 OR d2 OR d3)
    DigitalBus::NOR(d, d0, d1, d2,d3);
    cycles+=7;  // 4 reads, 2 op, 1 write
}

void SCAMP5::NOT(DREG &Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
    cycles+=4;  // 2 reads, 1 not, 1 write
}

void SCAMP5::OR(DREG &Rl, DREG &Rx) {
    // Rl := Rl OR Rx
    this->OR(Rl, Rl, Rx);
}

void SCAMP5::NOR(DREG &Rl, DREG &Rx) {
    // Rl := Rl NOR Rx
    this->NOR(Rl, Rl, Rx);
}

void SCAMP5::AND(DREG &Ra, DREG &Rx, DREG &Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->NOR(Ra, RF, RP);
}

void SCAMP5::NAND(DREG &Ra, DREG &Rx, DREG &Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->OR(Ra, RF, RP);
}

void SCAMP5::ANDX(DREG &Ra, DREG &Rb, DREG &Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, RF, Rb);
}

void SCAMP5::NANDX(DREG &Ra, DREG &Rb, DREG &Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, RF, Rb);
}

void SCAMP5::IMP(DREG &Rl, DREG &Rx, DREG &Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    this->NOT(RF, Ry);
    this->OR(RS, Rx, RF);
}

void SCAMP5::NIMP(DREG &Rl, DREG &Rx, DREG &Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(RF, Ry);
    this->NOR(RS, Rx, RF);
}

void SCAMP5::XOR(DREG &Rl, DREG &Rx, DREG &Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(RF, Ry);
    this->NOR(Rl, Rx, RF);
    this->NOT(RF, Rx);
    this->NOR(Rx, Ry, RF);
    this->OR(Rl, Rx);
}

void SCAMP5::WHERE(DREG &d) {
    // FLAG := d.
    this->FLAG.write(d.value());
    cycles+=2;  // 1 read, 1 write
}

void SCAMP5::WHERE(DREG &d0, DREG &d1) {
    // FLAG := d0 OR d1.
    DREG intermediate(d0.value().rows, d0.value().cols);
    this->OR(intermediate, d0, d1);
    this->FLAG.write(intermediate.value());
    cycles++;  // 1 write
}

void SCAMP5::WHERE(DREG &d0, DREG &d1, DREG &d2) {
    // FLAG := d0 OR d1 OR d2.
    DREG intermediate(d0.value().rows, d0.value().cols);
    this->OR(intermediate, d0, d1, d2);
    this->FLAG.write(intermediate.value());
    cycles++;  // 1 write
}

void SCAMP5::ALL() {
    // FLAG := 1, same as all.
    this->FLAG.set();
    cycles++;  // 1 write
}

void SCAMP5::SET(DREG &d0) {
    // d0 := 1
    d0.set();
    cycles++;  // 1 write
}

void SCAMP5::SET(DREG &d0, DREG &d1) {
    // d0, d1 := 1
    d0.set();
    d1.set();
    cycles+=2;  // 2 writes
}

void SCAMP5::SET(DREG &d0, DREG &d1, DREG &d2) {
    // 	d0, d1, d2 := 1
    d0.set();
    d1.set();
    d2.set();
    cycles+=3;  // 3 writes
}

void SCAMP5::SET(DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d0, d1, d2, d3 := 1
    d0.set();
    d1.set();
    d2.set();
    d3.set();
    cycles+=4;  // 4 writes
}

void SCAMP5::CLR(DREG &d0) {
    // d0 := 0
    d0.clear();
    cycles++;  // 1 write
}

void SCAMP5::CLR(DREG &d0, DREG &d1) {
    // d0, d1 := 0
    d0.clear();
    d1.clear();
    cycles+=2;  // 2 writes
}

void SCAMP5::CLR(DREG &d0, DREG &d1, DREG &d2) {
    // d0, d1, d2 := 0
    d0.clear();
    d1.clear();
    d2.clear();
    cycles+=3;  // 3 writes
}

void SCAMP5::CLR(DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // 	d0, d1, d2, d3 := 0
    d0.clear();
    d1.clear();
    d2.clear();
    d3.clear();
    cycles+=4;  // 4 writes
}

void SCAMP5::MOV(DREG &d, DREG &d0) {
    // d := d0
    DigitalBus::MOV(d, d0);
    cycles+=2;  // 1 read, 1 write
}

void SCAMP5::MUX(DREG &Rl, DREG &Rx, DREG &Ry, DREG &Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    DigitalBus::MUX(Rl, Rx, Ry, Rz);
    cycles+=4;  // 3 reads, 1 write, some op?
}

void SCAMP5::CLR_IF(DREG &Rl, DREG &Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalBus::CLR_IF(Rl, Rx);
    cycles+=2;  // 1 read, up to 1 write, some op for if?
}

void SCAMP5::REFRESH(DREG &Rl) {
    // 	refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds) without any operations
}

void SCAMP5::DNEWS0(DREG &d, DREG &d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
}

void SCAMP5::DNEWS1(DREG &d, DREG &d0) {
    // 	d := d0_dir, direction selected by R1, R2, R3, R4
}

void SCAMP5::DNEWS(DREG &Ra, DREG &Rx, int dir, bool boundary) {
    // digital neighbour OR, Ra := Rx_dir1 OR Rx_dir2 ...; (also modify R1 R2 R3 R4).
    this->CLR(RS, RW, RN, RE);
    // set multiple DREG& (upto 4) can be done via one icw
    if(dir&south){
        this->SET(RS);
    }
    if(dir&west){
        this->SET(RW);
    }
    if(dir&north){
        this->SET(RN);
    }
    if(dir&east){
        this->SET(RE);
    }

    if(boundary){
        this->DNEWS1(Ra,Rx);
    }else{
        this->DNEWS0(Ra,Rx);
    }
}

void SCAMP5::PROP0() {
    // async-propagation on R12, masked by R0
}

void SCAMP5::PROP1() {
    // async-propagation on R12, masked by R0 when boundaries are considered '1'
}

void SCAMP5::print_stats(const CycleCounter& counter) {
    // TODO move
    json j;
    j["Total number of cycles"] = counter.get_cycles();
    j["Equivalent in seconds"] = counter.to_seconds(stats::CLOCK_RATE);

    // this->array.print_stats(counter);
    this->array.write_stats(counter, j);
    std::cout << std::setw(2) << j << std::endl;
    std::ofstream file_out;
    std::cout << std::filesystem::current_path().string() << std::endl;
    file_out.open (std::filesystem::current_path().string() + "/output.json");
    file_out << std::setw(2) << j;
    file_out.close();
}

void SCAMP5::scamp5_get_image(AREG &yf, AREG &yh, int gain) {
    // put the exposure result in PIX to AREGs and reset PIX
    // yf	full range [-128,127]
    // yh	half range [0,127]
    // gain	(optional) gain [1,5]
    // get_image(A, B, 0|1)
    // TODO improve capture rate. Multithreading needed?

    #ifdef USE_RUNTIME_CHECKS
        if (gain < 1 || gain > 5) {
            std::cout << "[Warning] Gain should be in [1,5]" << std::endl;
        }
    #endif

    all();
    this->pe.photodiode.read(PIX);
    bus(NEWS, PIX);
    rpix();
    this->pe.photodiode.read(PIX);
    bus(yf, NEWS, PIX);
    bus(NEWS, yf);

    for (int i = 1; i < gain; i++) {
        bus(yh, NEWS);
        bus(yf, NEWS);
        bus(NEWS, yh, yf);
        bus(yh, NEWS, E);
        where(yh);
        bus(NEWS, E);
        all();
    }

    bus(yh, NEWS);
    bus(yf, NEWS);
    rpix();
    this->pe.photodiode.read(PIX);
    bus(NEWS, yh, yf, PIX);
    bus(yf, NEWS);

}

void SCAMP5::scamp5_in(AREG &areg, int8_t value, AREG* temp) {
    // load an analog value to the AREG with error&noise correction
    // TODO noise
    // TODO Pointer instead of reference. Need member as default somehow
    if (temp == nullptr) {
        temp = &NEWS;
    }
    IN.write(value);
    cycles++;
    bus(*temp, IN);
    bus(areg, *temp);
}

void SCAMP5::scamp5_load_in(AREG &areg, int8_t value, AREG *temp) {
    // load a analog value to the AREG plane without error&noise correction
    // TODO noise
    if (temp == nullptr) {
        temp = &NEWS;
    }
    IN.write(value);
    cycles++;
    bus(*temp, IN);
    bus(areg, *temp);
}

void SCAMP5::scamp5_load_in(int8_t value) {
    // load a analog value to IN without error&noise correction
    // TODO noise
    IN.write(value);
    cycles++;
}

void SCAMP5::scamp5_load_dac(AREG &areg, uint16_t value, AREG *temp) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	(optional) temporary kernel register to be used in the function
    if (temp == nullptr) {
        temp = &NEWS;
    }
    IN.write(value);
    cycles++;
    bus(*temp, IN);
    bus(areg, *temp);
}

void SCAMP5::scamp5_load_dac(uint16_t value) {
    // load an analog value to IN using a raw DAC value
    // TODO What is with the range of values here. Why can some registers hold a much larger range?
    IN.write(value);
    cycles++;
}

void SCAMP5::scamp5_shift(AREG &areg, int h, int v) {
    // shift an AREG image
    // this->pe.analogue_bus.

    // scamp5_shift(A, 1, 0)
    // bus(EAST, A)
    // bus(A, NEWS)

    // scamp5_shift(A, 2, 0)
    // bus(EAST, A)
    // bus(A, WEST)

    // scamp5_shift(A, 3, 0)
    // bus(EAST, A)
    // bus(A, WEST)
    // bus(EAST, A)
    // bus(A, NEWS)

    // scamp5_shift(A, 4, 0)
    // bus(EAST, A)
    // bus(A, WEST)
    // bus(EAST, A)
    // bus(A, WEST)

    // Horizontal shift
//    AREG EAST(1, 1);
//    AREG WEST(1, 1);
//    if (h != 0) {
//        AREG *direction = &(h > 0 ? EAST : WEST);
//        AREG *end = (h % 2 == 0 ? direction : &NEWS);
//
//        bus(*direction, areg);
//        for (int i = 2; i < h; i++) {
//            bus(A, WEST)
//            bus(EAST, A)
//        }
//        bus(areg, *end);
//
//
//
//    }

    // Vertical shift



}

void SCAMP5::scamp5_diffuse(AREG &target, int iterations, bool vertical, bool horizontal, AREG *t0) {
    // diffuse an AREG image
    // TODO is this the same as Gaussian blur?
    void (SCAMP5::*blur_func)(AREG &, AREG &);
    if (horizontal && vertical) {
        blur_func = &SCAMP5::blur;
    } else if (horizontal) {
        blur_func = &SCAMP5::blurh;
    } else {
        blur_func = &SCAMP5::blurv;
    }

    if (t0 == nullptr) {
        t0 = &NEWS;
    }

    (*this.*blur_func)(*t0, target);
    (*this.*blur_func)(target, *t0);
    for (int i = 1; i < iterations; i++) {
        (*this.*blur_func)(*t0, target);
        (*this.*blur_func)(target, *t0);
    }

}

uint8_t SCAMP5::scamp5_read_areg(AREG &areg, uint8_t r, uint8_t c) {
    // read a single pixel
    // TODO check that the value is properly mapped to uint8_t from CV_16U
    return areg.value().at<uint8_t>(r, c);
}

uint32_t SCAMP5::scamp5_global_sum_16(AREG &areg, uint8_t *result16v) {
    // get the AREG sum level using a set of 4x4 sparse grid.
    // When result16v is a NULL pointer, the function will return sum of the data.
    // The result is not equal to the actual sum of all pixels of the AREG in the area,
    // but it is proportional to the actual sum.
    // This function takes roughly 14 usec (on the scamp5 device) to execute.
    // TODO should not be exact
    uint32_t sum = 0;
    int buf_index = 0;
    int cs = SCAMP_WIDTH / 4;
    int rs = SCAMP_HEIGHT / 4;
    for (int col = 0; col < SCAMP_WIDTH; col+=cs) {
        for (int row = 0; row < SCAMP_HEIGHT; row+=rs) {
            int val = cv::sum(areg.value()(cv::Rect(col, row, col+cs, row+rs)))[0];
            if (result16v == nullptr) {
                sum += val;
            } else {
                result16v[buf_index++] = val;
            }
        }
    }
    return sum;
}

uint32_t SCAMP5::scamp5_global_sum_64(AREG &areg, uint8_t *result64v) {
    // get the AREG sum level with a better resolution
    // This function achieves similar functionally as the "global_sum_16" version, but the grid used is 8x8.
    // As a consequence, the result has higher resolution but it takes longer to execute (roughly 28 usec on the scamp5).
    uint32_t sum = 0;
    int buf_index = 0;
    int cs = SCAMP_WIDTH / 8;
    int rs = SCAMP_HEIGHT / 8;
    for (int col = 0; col < SCAMP_WIDTH; col+=cs) {
        for (int row = 0; row < SCAMP_HEIGHT; row+=rs) {
            int val = cv::sum(areg.value()(cv::Rect(col, row, col+cs, row+rs)))[0];
            if (result64v == nullptr) {
                sum += val;
            } else {
                result64v[buf_index++] = val;
            }
        }
    }
    return sum;
}

uint8_t SCAMP5::scamp5_global_sum_fast(AREG &areg) {
    // get approximate sum level of the whole AREG plane
    // TODO should be approximate sum not exact. Also need to abstract away cv call
    return cv::sum(areg.value())[0];
}

uint8_t SCAMP5::scamp5_global_sum_sparse(AREG &areg, uint8_t r, uint8_t c, uint8_t rx, uint8_t cx) {
    // get sum level of the pixels selected using a pattern
    // This result is less probable to saturate because it only counts a quarter of the pixels in the AREG plane (by default)
    unsigned int r_mask = ((~rx) & (SCAMP_HEIGHT-1));
    unsigned int c_mask = ((~cx) & (SCAMP_WIDTH-1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    uint8_t sum = 0;

    for (unsigned int row_index = 0; row_index < SCAMP_WIDTH; row_index++) {
        for (unsigned int col_index = 0; col_index < SCAMP_HEIGHT; col_index++) {
            if (((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                sum += areg.value().at<uint8_t>(row_index, col_index);
            }
        }
    }

    return sum;
}

void SCAMP5::scamp5_shift(DREG &dreg, int h, int v, const int boundary) {
    // shift an DREG image
}

int SCAMP5::scamp5_global_or(DREG &dreg, uint8_t r, uint8_t c, uint8_t rx, uint8_t cx) {
    // get OR result of all pixels in a DREG plane. 0 if all pixel is 0, non-zero otherwise.
    // The default mask pattern parameters gives the whole image.
    // scamp5_load_pattern can be used to work out the correct parameter for a desired mask pattern,
    //TODO abstraction
    unsigned int r_mask = ((~rx) & (SCAMP_HEIGHT-1));
    unsigned int c_mask = ((~cx) & (SCAMP_WIDTH-1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    uint8_t val = 0;

    for (unsigned int row_index = 0; row_index < SCAMP_WIDTH; row_index++) {
        for (unsigned int col_index = 0; col_index < SCAMP_HEIGHT; col_index++) {
            if (((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                val |= dreg.value().at<uint8_t>(row_index, col_index);
            }
        }
    }

    return val;
}

int SCAMP5::scamp5_global_count(DREG &dreg, AREG &temp, int options) {
    // get an estimation of the number of '1's in a DREG plane
    // Internally this function converts the DREG to AREG using suitable analog levels to represent '0's and '1's.
    // Then a AREG global sum is done and the result is uniformed into [0,4095].
    // TODO options and estimation
    dreg.value().convertTo(temp.value(), 255, 0);
    int total = cv::sum(temp.value())[0];
    double min_val, max_val;
    cv::minMaxLoc(temp.value(), &min_val, &max_val);
    return utility::normalise(total, min_val, max_val, 0, 4096);
}

void SCAMP5::scamp5_flood(DREG &dreg_target, DREG &dreg_mask, int boundary, int iterations, bool use_local) {
    // flooding a DREG image with '1's from the '1's in the DREG image
    // dreg_target	the DREG to be flooded
    // dreg_mask	the control DREG to inhibit the flood
    // boundary	    boundary value
    // iterations	(optional) number of iterations
    // use_local	(optional) use pre-exist R1-R4 to control the flood direction
    // The target DREG will be flooded by '1's except those areas that are closed by '0's in the control register.
    // To increase the travelling distance of the flood, the number of iteration should be increased.

    // dreg_mask can contain many different seeds. Each one is called individually to start the flood fill

    //TODO ensure mask is correct size after floodfill operation

    std::vector<cv::Point> seeds;   // locations of white pixels in the original image
    cv::findNonZero(dreg_target.value(), seeds);

    uint8_t fillValue = 1;

    // Add additional pixel to each side
    cv::Mat mask;
    cv::copyMakeBorder(dreg_mask.value(), mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);

    dreg_mask.value() = 1 - dreg_mask.value();

    for (auto& seed : seeds){
        cv::floodFill(dreg_mask.value(), mask, seed, cv::Scalar(1) ,nullptr, cv::Scalar(0), cv::Scalar(1), 4);
    }


}

void SCAMP5::scamp5_load_point(DREG &dr, uint8_t r, uint8_t c) {
    // set a single pixel on a DREG image to 1, the rest to 0
    dr.value().setTo(0);
    dr.value().at<uint8_t>(r, c) = 1;
}

void SCAMP5::scamp5_load_rect(DREG &dr, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1) {
    // set a rectangle area on a DREG image to 1, the rest to 0
    // r0	pixel row index of the top right corner
    // c0	pixel column index of the top right corner
    // r1	pixel row index of the bottom left corner
    // c1	pixel column index of the bottom left corner

    // TODO In SCAMP5, (0,0) is in the top right. Need to convert this everywhere
    // OpenCV has (0,0) at top left.

    // TODO check
    #ifndef NDEBUG
        assert(r0 - r1 > 0);
        assert(r0 - r1 > 0);
    #endif
    int width = c0 - c1;
    int height = r0 - r1;
    dr.clear();
    dr.value()(cv::Rect(r0, c1, width, height)).setTo(1);
}

void SCAMP5::scamp5_load_pattern(DREG &dr, uint8_t r, uint8_t c, uint8_t rx, uint8_t cx) {
    // set those pixels with matching address to 1, the rest to 0
    // To mask out certain bits in the column/row address allow a match to occur periodically.
    // For example, set mask to 192 (11000000b) makes the four following address match the value 3 (00000011b): 3(00000011b), 67(01000011b), 131(10000011b) and 195(11000011b).
    // TODO abstraction
    dr.clear();

    unsigned int r_mask = ((~rx) & (SCAMP_HEIGHT-1));
    unsigned int c_mask = ((~cx) & (SCAMP_WIDTH-1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    for (unsigned int row_index = 0; row_index < SCAMP_WIDTH; row_index++) {
        for (unsigned int col_index = 0; col_index < SCAMP_HEIGHT; col_index++) {
            if (((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                dr.value().at<uint8_t>(row_index, col_index) = 1;
            }
        }
    }

}

void SCAMP5::scamp5_select_point(uint8_t r, uint8_t c) {
    // same as scamp5_load_point, but put the point in SELECT
    scamp5_load_point(SELECT, r, c);
}

void SCAMP5::scamp5_select_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1) {
    // same as scamp5_load_rect, but put the rectangle in RECT (also modify SELECT)
    // TODO why is SELECT modified?
    scamp5_load_rect(RECT, r0, c0, r1, c1);
}

void SCAMP5::scamp5_select_pattern(uint8_t r, uint8_t c, uint8_t rx, uint8_t cx) {
    // same as scamp5_load_pattern, but put the pattern in SELECT
    // TODO check
    scamp5_load_pattern(SELECT, r, c, rx, cx);
}

void SCAMP5::scamp5_select_col(uint8_t c) {
    // select column
    for (unsigned int row_index = 0; row_index < SCAMP_WIDTH; row_index++) {
        for (unsigned int col_index = 0; col_index < SCAMP_HEIGHT; col_index++) {
            if (col_index == c) {
                SELECT.value().at<uint8_t>(row_index, col_index) = 1;
            }
        }
    }
}

void SCAMP5::scamp5_select_row(uint8_t r) {
    // select row
    for (unsigned int row_index = 0; row_index < SCAMP_WIDTH; row_index++) {
        for (unsigned int col_index = 0; col_index < SCAMP_HEIGHT; col_index++) {
            if (row_index == r) {
                SELECT.value().at<uint8_t>(row_index, col_index) = 1;
            }
        }
    }
}

void SCAMP5::scamp5_select_colx(uint8_t cx) {
    // select column mask
}

void SCAMP5::scamp5_select_rowx(uint8_t rx) {
    // select row mask
}

void SCAMP5::scamp5_draw_begin(DREG &dr) {
    // targeting a DREG image to a series of drawing operations
    scratch = &dr;
}

void SCAMP5::scamp5_draw_end() {
    // end the drawing operations
    scratch = nullptr;
}

void SCAMP5::scamp5_draw_pixel(uint8_t r, uint8_t c) {
    // draw a point, wrap around if it's outside the border
    scratch->value().at<uint8_t>(r%SCAMP_HEIGHT, c%SCAMP_WIDTH) = 1;
}

bool SCAMP5::scamp5_draw_point(int r, int c) {
    // draw a point when its position is within the image
    // returns whether the point is inside the image and drawn
    if (r >= SCAMP_HEIGHT || c >= SCAMP_WIDTH) {
        return false;
    }
    scratch->value().at<uint8_t>(r, c) = 1;
    return true;
}

void SCAMP5::scamp5_draw_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1) {
    // draw a filled rectangle
    // r0 pixel row index of the top right corner
    // c0 pixel column index of the top right corner
    // r1 pixel row index of the bottom left corner
    // c1 pixel column index of the bottom left corner
    // FIXME issue with coordinates
    int width = c1 - c0;
    int height = r1 - r0;
    scratch->clear();
    scratch->value()(cv::Rect(r0, c1, width, height)).setTo(1);
}

void SCAMP5::scamp5_draw_line(int r0, int c0, int r1, int c1, bool repeat) {
    // draw a line
    //r0 - starting point row coordinate
    //c0 - starting point column coordinate
    //r1 - finishing point row coordinate
    //c1 - finishing point column coordinate
    //repeat - whether to wrap around when point goes outside the image
    // TODO wrap around
    cv::line(scratch->value(), {r0, c0}, {r1, c1}, 1);

}

void SCAMP5::scamp5_draw_circle(int x0, int y0, int radius, bool repeat) {
    // draw a non-filled circle - uses Bresenham's Midpoint Circle algorithm
    // repeat is for controlling wrap around
    // TODO need to use scamp5_load_point like existing sim
    #ifdef USE_RUNTIME_CHECKS
        if (scratch == nullptr) {
            std::cerr << "Drawing register has not been set" << std::endl;
        }
    #endif

    int f = 1 - radius;
    int ddf_x = 1;
    int ddf_y = -2 * radius;
    int x = 0;
    int y = radius;

    scratch->value().at<uint8_t>(y0 + radius, x0) = 1;
    scratch->value().at<uint8_t>(y0 - radius, x0) = 1;
    scratch->value().at<uint8_t>(y0, x0 + radius) = 1;
    scratch->value().at<uint8_t>(y0, x0 - radius) = 1;

    while (x < y) {
        if (f >= 0) {
            y -= 1;
            ddf_y += 2;
            f += ddf_y;
        }

        x += 1;
        ddf_x += 2;
        f += ddf_x;

        scratch->value().at<uint8_t>(y0 + y, x0 + x) = 1;
        scratch->value().at<uint8_t>(y0 + y, x0 - x) = 1;
        scratch->value().at<uint8_t>(y0 - y, x0 + x) = 1;
        scratch->value().at<uint8_t>(y0 - y, x0 - x) = 1;
        scratch->value().at<uint8_t>(y0 + x, x0 + y) = 1;
        scratch->value().at<uint8_t>(y0 + x, x0 - y) = 1;
        scratch->value().at<uint8_t>(y0 - x, x0 + y) = 1;
        scratch->value().at<uint8_t>(y0 - x, x0 - y) = 1;
    }
}

void SCAMP5::scamp5_draw_negate() {
    // do a binary inversion of the DREG image.
    // TODO abstraction
    scratch->value() = 1 - scratch->value();
}

// Image Readout

void SCAMP5::scamp5_scan_areg(AREG &areg, uint8_t *buffer, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs,
                              uint8_t cs) {
    // scan a customized grid of pixels in an AREG image
    // Note, the result image is stored in column-major format, starting from top right.
    // i.e. "buffer[3]" is the pixel the on 1st column right, 4th row down. This applies to all "scamp5_scan_areg_*" series functions.
    #ifdef USE_RUNTIME_CHECKS
        if (r1 < r0) {
            std::cout << "[Warning] Row end is before row start" << std::endl;
        }
        if (c1 < c0) {
            std::cout << "[Warning] Column end is before column start" << std::endl;
        }
    #endif
    // TODO check
    int buf_index = 0;
    for (int col = c0; col < c1; col+=cs) {
        for (int row = r0; row < r1; row+=rs) {
            buffer[buf_index++] = areg.value().at<uint8_t>(row, col);
        }
    }
}

void SCAMP5::scamp5_scan_areg_8x8(AREG &areg, uint8_t *result8x8) {
    // scan a 8x8 grid of pixels in an AREG image
    // This function is slightly faster and more accurate than scamp5_scan_areg.
    // TODO currently assuming this takes beginning of each cell in 8x8 grid. Might be center instead
    // TODO check RowMajor/ColMajor stuff
    int buf_index = 0;
    int cs = SCAMP_WIDTH / 8;
    int rs = SCAMP_HEIGHT / 8;
    for (int col = 0; col < SCAMP_WIDTH; col+=cs) {
        for (int row = 0; row < SCAMP_HEIGHT; row+=rs) {
            result8x8[buf_index++] = areg.value().at<uint8_t>(row, col);
        }
    }
}

void SCAMP5::scamp5_scan_areg_mean_8x8(AREG &areg, uint8_t *result8x8) {
    // divide the AREG image into 8x8 square blocks, and get the average of each block
    // result8x8 - pointer to a buffer to store the results
    // FIXME with proper setting of (0, 0)
    int buf_index = 0;
    int step = SCAMP_HEIGHT/8;
    for (int col = 0; col < SCAMP_WIDTH; col+=step) {
        for (int row = 0; row < SCAMP_HEIGHT; row+=step) {
            result8x8[buf_index++] = cv::sum(areg.value()(cv::Rect(col, row, col+step, row+step)))[0]/(step*step);
        }
    }

}

void SCAMP5::scamp5_scan_dreg(DREG &dreg, uint8_t *mem, uint8_t r0, uint8_t r1) {
    // scan DREG image, store the result in a buffer
    // mem - pointer to a buffer
    // r0 - first row index
    // r1 - last row index
    // The size of the buffer need to be a least 32 times the number of rows to scan.
    // Thus, a full DREG image requires a buffer of 8192 bytes.
    // TODO check if it should be (row, col) or (col, row)
    // TODO check impl
    int buf_index = 0;
    for (uint32_t row_index = r0; row_index <= r1; row_index++) {
        // Read 8 values at a time to make up a byte
        for (int col_index = 0; col_index < SCAMP_WIDTH; col_index+=8) {
            uint8_t b0 = dreg.value().at<uint8_t>(row_index, col_index);
            uint8_t b1 = dreg.value().at<uint8_t>(row_index, col_index+1);
            uint8_t b2 = dreg.value().at<uint8_t>(row_index, col_index+2);
            uint8_t b3 = dreg.value().at<uint8_t>(row_index, col_index+3);
            uint8_t b4 = dreg.value().at<uint8_t>(row_index, col_index+4);
            uint8_t b5 = dreg.value().at<uint8_t>(row_index, col_index+5);
            uint8_t b6 = dreg.value().at<uint8_t>(row_index, col_index+6);
            uint8_t b7 = dreg.value().at<uint8_t>(row_index, col_index+7);
            uint8_t value = (b0 << 7) | (b1 << 6) | (b2 << 5) | (b3 << 4) | (b4 << 3) | (b5 << 2) | (b6 << 1) | (b7 << 0);
            mem[buf_index++] = value;
        }
    }
}

void SCAMP5::scamp5_scan_events(DREG &dreg, uint8_t *mem, uint16_t max_num, uint8_t h_dir, uint8_t v_dir) {
    // scan the coordinates of all '1's in a DREG image and store the result in a buffer
    // mem - pointer to a buffer of (max_num*2) bytes
    // max_num - maximum number of events
    // h_dir - horizontal order of scanning, '0' means from column 0 to column 255
    // v_dir - vertical order of scanning, '0' means from row 0 to row 255
    // Up-to 4000 events scan be scanned. Note, the execution time of this function is proportional to max_num. In the result, the first occurrence of (0,0) indicates the end of effective events.
    // writes to mem as (col, row)
    // TODO Row/Col confusion. Make consistent
    // FIXME with proper setting of (0, 0)
    // TODO does not take scanning direction into account. Rewrite
    std::vector<cv::Point> locations;   // output, locations of non-zero pixels
    cv::findNonZero(dreg.value(), locations);
    int buf_index = 0;
    for (auto& p : locations) {
        if (buf_index == 2*max_num) break;
        mem[buf_index++] = p.x;
        mem[buf_index++] = p.y;
    }
}

void SCAMP5::scamp5_scan_boundingbox(DREG &dr, uint8_t *vec4v) {
    // scan the bounding box coordinates of '1's in a DREG image
    // The coordinates are two points: the top-right and the bottom-left corners of the axis-aligned bounding box (AABB) of '1's in the DREG.
    // vec4v - pointer to a buffer of 4 byte
}


