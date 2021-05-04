//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"

#include <simulator/memory/sram6t_cell.h>
#include <simulator/util/utility.h>
#include <rttr/registration>

#include <filesystem>
#include <iostream>
#include <ostream>
#include <utility>
#include "simulator/external/parser.h"

void SCAMP5::init() {
    // Registers used often in instructions
    pe = this->get_component<ProcessingElement>("pe");
    PIX = pe->get_analogue_register("PIX");
    IN = pe->get_analogue_register("IN");
    NEWS = pe->get_analogue_register("NEWS");
    A = pe->get_analogue_register("A");
    B = pe->get_analogue_register("B");
    C = pe->get_analogue_register("C");
    D = pe->get_analogue_register("D");
    E = pe->get_analogue_register("E");
    F = pe->get_analogue_register("F");

    FLAG = pe->get_digital_register("FLAG");
    RF = pe->get_digital_register("R0");
    RS = pe->get_digital_register("R1");
    RW = pe->get_digital_register("R2");
    RN = pe->get_digital_register("R3");
    RE = pe->get_digital_register("R4");
    S0 = pe->get_digital_register("R5");
    S1 = pe->get_digital_register("R6");
    S2 = pe->get_digital_register("R7");
    S3 = pe->get_digital_register("R8");
    S4 = pe->get_digital_register("R9");
    S5 = pe->get_digital_register("R10");
    S6 = pe->get_digital_register("R11");
    RP = pe->get_digital_register("R12");
    SELECT = pe->get_digital_register("SELECT");
    RECT = pe->get_digital_register("RECT");

    // Initially all PEs are active
    FLAG->write(1);

    FLAG->change_memory_type(MemoryType::SRAM6T);

    intermediate_a = std::make_shared<AREG>(this->rows_, this->cols_);
    intermediate_a2 = std::make_shared<AREG>(this->rows_, this->cols_);
    intermediate_d = std::make_shared<DREG>(this->rows_, this->cols_);
}

void SCAMP5::nop() { this->update_cycles(1); }

void SCAMP5::rpix() {
    // reset *PIX
    this->pe->get_pixel()->reset();
    this->update_cycles(1);
}

void SCAMP5::get_image(const std::shared_ptr<AREG>& y) {
    // y := half-range image, and reset *PIX
    this->pe->get_pixel()->read(*PIX);
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void SCAMP5::get_image(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& h) {
    // y := full-range image, h := negative half-range image, and reset *PIX
    this->pe->get_pixel()->read(*PIX);
    this->bus(NEWS, PIX);
    this->bus(h, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, h, NEWS, PIX);
}

void SCAMP5::respix() {
    // reset *PIX
    this->rpix();
    this->rpix();
    this->nop();
}

void SCAMP5::respix(const std::shared_ptr<AREG>& y) {
    // reset *PIX, keep its reset level in y
    this->rpix();
    this->rpix();
    this->nop();
    this->pe->get_pixel()->read(*PIX);
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void SCAMP5::getpix(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& pix_res) {
    // y := half-range image, supplying the reset level of *PIX
    this->pe->get_pixel()->read(*PIX);
    this->bus(NEWS, PIX);
    this->bus(y, NEWS, pix_res);
}

void SCAMP5::getpix(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& h, const std::shared_ptr<AREG>& pix_res) {
    // y := full-range, h := half-range image, supplying the reset level of *PIX
    this->pe->get_pixel()->read(*PIX);
    this->bus(h, PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

void SCAMP5::bus(const std::shared_ptr<AREG>& a) {
    // a = 0 + error
    this->pe->analogue_bus.bus(*a, *FLAG);
    this->update_cycles(1);
}

void SCAMP5::bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0) {
    // a = -a0 + error
    this->pe->analogue_bus.bus(*a, *a0, *FLAG);
    this->update_cycles(1);
}

void SCAMP5::bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1) {
    // a = -(a0 + a1) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *FLAG);
    this->update_cycles(4); // 2 reads, 1 add, 1 write
}

void SCAMP5::bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2) {
    // a = -(a0 + a1 + a2) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *FLAG);
    this->update_cycles(5);  // 3 reads, 1 add, 1 write
}

void SCAMP5::bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2, const std::shared_ptr<AREG>& a3) {
    // a = -(a0 + a1 + a2 + a3) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *a3, *FLAG);
    this->update_cycles(6);  // 4 reads, 1 add, 1 write
}

void SCAMP5::bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b) {
    // a,b = 0 + error
    this->pe->analogue_bus.bus2(*a, *b, *FLAG);
    this->update_cycles(2);  // 2 writes
}

void SCAMP5::bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& a0) {
    // a,b = -0.5*a0 + error + noise
    this->pe->analogue_bus.bus2(*a, *b, *a0, *FLAG);
    this->update_cycles(3);  // 1 read, 2 writes
}

void SCAMP5::bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1) {
    // a,b = -0.5*(a0 + a1) + error + noise
    this->pe->analogue_bus.bus2(*a, *b, *a0, *a1, *FLAG);
    this->update_cycles(5);  // 2 reads, 1 add, 2 writes
}

void SCAMP5::bus3(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& c, const std::shared_ptr<AREG>& a0) {
    // a,b,c = -0.33*a0 + error + noise
    this->pe->analogue_bus.bus3(*a, *b, *c, *a0, *FLAG);
    this->update_cycles(2);  // 1 read, 3 writes
}

void SCAMP5::where(const std::shared_ptr<AREG>& a) {
    // FLAG := a > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a);
    this->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::where(const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1) {
    // FLAG := (a0 + a1) > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a0, *a1);
    this->update_cycles(4);  // 2 reads, 1 add, 1 write
}

void SCAMP5::where(const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2) {
    // FLAG := (a0 + a1 + a2) > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a0, *a1, *a2);
    this->update_cycles(5);  // 3 reads, 1 add, 1 write
}

void SCAMP5::all() {
    // FLAG := 1.
    this->FLAG->set();
    this->update_cycles(1);  // 1 write
}

void SCAMP5::mov(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0) {
    // y = x0
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

void SCAMP5::res(const std::shared_ptr<AREG>& a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void SCAMP5::res(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void SCAMP5::add(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void SCAMP5::add(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, const std::shared_ptr<AREG>& x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void SCAMP5::sub(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void SCAMP5::neg(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void SCAMP5::abs(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0) {
    // y = |x0|
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void SCAMP5::div(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::div(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2, const std::shared_ptr<AREG>& x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::diva(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void SCAMP5::divq(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

void SCAMP5::movx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir) {
    // y = x0_dir
    this->bus(intermediate_a, x0);
    switch(dir) {
        case south:
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        case north:
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS);
    this->update_cycles(1);  // movement?
}

void SCAMP5::mov2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir2 (note: this only works when FLAG is "all")
    this->bus(intermediate_a, x0);
    switch(dir) {
        case north: {
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        }
        case east: {
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        }
        case west: {
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        }
        case south: {
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        }
        case alldir: {
            std::cerr << "Unhandled direction" << std::endl;
            break;
        }
    }
    switch(dir2) {
        case north: {
            this->pe->analogue_bus.get_north(*intermediate_a2, *NEWS, 1, origin_);
            break;
        }
        case east: {
            this->pe->analogue_bus.get_east(*intermediate_a2, *NEWS, 1, origin_);
            break;
        }
        case west: {
            this->pe->analogue_bus.get_west(*intermediate_a2, *NEWS, 1, origin_);
            break;
        }
        case south: {
            this->pe->analogue_bus.get_south(*intermediate_a2, *NEWS, 1, origin_);
            break;
        }
        case alldir: {
            std::cerr << "Unhandled direction" << std::endl;
            break;
        }
    }
    this->bus(y, intermediate_a2);
    this->update_cycles(2);  // movement
}

void SCAMP5::addx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, news_t dir) {
    // y := x0_dir + x1_dir
    this->bus(intermediate_a, x0, x1);
    switch(dir) {
        case north:
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }

    this->bus(y, NEWS);
    this->update_cycles(1);  //movement
}

void SCAMP5::add2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    this->bus(intermediate_a, x0, x1);
    switch(dir) {
        case north:
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch(dir2) {
        case north:
            this->pe->analogue_bus.get_north(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, intermediate_a2);
    this->update_cycles(2);  // movement
}

void SCAMP5::subx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, const std::shared_ptr<AREG>& x1) {
    // y := x0_dir - x1
    this->bus(intermediate_a, x0);
    switch(dir) {
        case north:
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS, x1);
    this->update_cycles(1);  // movement
}

void SCAMP5::sub2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, news_t dir2, const std::shared_ptr<AREG>& x1) {
    // y := x0_dir_dir2 - x1
    this->bus(intermediate_a, x0);
    switch(dir) {
        case north:
            this->pe->analogue_bus.get_north(*NEWS, *intermediate_a, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*NEWS, *intermediate_a, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*NEWS, *intermediate_a, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*NEWS, *intermediate_a, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch(dir2) {
        case north:
            this->pe->analogue_bus.get_north(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case east:
            this->pe->analogue_bus.get_east(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case west:
            this->pe->analogue_bus.get_west(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case south:
            this->pe->analogue_bus.get_south(*intermediate_a2, *NEWS, 1, origin_);
            break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, intermediate_a2, x1);
    this->update_cycles(2);  // movement
}

void SCAMP5::blurset() {
    // need to be used once ahead of a blur instruction
    // TODO check
    this->get_component<ProcessingElement>("pe")->get_digital_register("RS")->set();
    this->get_component<ProcessingElement>("pe")->get_digital_register("RW")->set();
    this->update_cycles(2);  // 2 writes
}

void SCAMP5::blur(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0) {
    // blur a0 into a
    // full blur across array.
    cv::GaussianBlur(a0->read(), a->read(), cv::Size(3, 3), 0);
    cv::bitwise_not(a->read(), a->read());
    this->update_cycles(sqrt(rows_ * cols_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::blurh(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0) {
    // analog asynchronized blur, horizontal only
    // row vector for x direction which is horizontal?
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat horizontal_kernel = cv::Mat(1, 6, CV_32F, gaussian_1d);

    cv::filter2D(a0->read(), a->read(), -1, horizontal_kernel);
    cv::bitwise_not(a->read(), a->read());
    this->update_cycles(sqrt(rows_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::blurv(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0) {
    // analog asynchronized blur, vertical only
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat vertical_kernel = cv::Mat(6, 1, CV_32F, gaussian_1d);

    cv::filter2D(a0->read(), a->read(), -1, vertical_kernel);
    cv::bitwise_not(a->read(), a->read());
    this->update_cycles(sqrt(cols_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::gauss(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // blur x into y with constant number of iterations (x and y can be same
    // AREG), require R1 and R2 to be set properly
    blur(NEWS, x);
    blur(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blur(NEWS, y);
        blur(y, NEWS);
    }
}

void SCAMP5::gaussh(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // horizontally blur x into y with constant number of iterations (x and y
    // can be same AREG), require R1 and R2 to be set properly
    blurh(NEWS, x);
    blurh(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blurh(NEWS, y);
        blurh(y, NEWS);
    }
}

void SCAMP5::gaussv(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // vertically blur x into y with constant number of iterations (x and y can
    // be same AREG), require R1 and R2 to be set properly
    blurv(NEWS, x);
    blurv(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blurv(NEWS, y);
        blurv(y, NEWS);
    }
}

void SCAMP5::newsblur(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // blur x into y with constant number of iterations using neighbour mixing
    // (x and y can be same AREG)
    int data[3][3] = {{0, 1, 0}, {1, 0, 1}, {0, 1, 0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
    cv::bitwise_not(y->read(), y->read());

    for(int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
        cv::bitwise_not(y->read(), y->read());
        this->update_cycles(sqrt(rows_ * cols_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::newsblurh(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // horizontally blur x into y with constant number of iterations using
    // neighbour mixing (x and y can be same AREG)
    int data[3][3] = {{0, 0, 0}, {1, 0, 1}, {0, 0, 0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
    cv::bitwise_not(y->read(), y->read());

    for(int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
        cv::bitwise_not(y->read(), y->read());
        this->update_cycles(sqrt(rows_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::newsblurv(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, const int iterations) {
    // vertically blur x into y with constant number of iterations using
    // neighbour mixing (x and y can be same AREG)
    int data[3][3] = {{0, 1, 0}, {0, 0, 0}, {0, 1, 0}};
    cv::Mat neighbour_kernel = cv::Mat(3, 3, CV_32F, data);

    bus(NEWS, x);
    cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
    cv::bitwise_not(y->read(), y->read());

    for(int i = 1; i < iterations; i++) {
        bus(NEWS, y);
        cv::filter2D(NEWS->read(), y->read(), -1, neighbour_kernel);
        cv::bitwise_not(y->read(), y->read());
        this->update_cycles(sqrt(cols_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1) {
    // d := d0 OR d1
    this->pe->local_read_bus.OR(*d, *d0, *d1);
    this->update_cycles(4); // 2 reads, 1 op, 1 write
}

void SCAMP5::OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2) {
    // d := d0 OR d1 OR d2
    this->pe->local_read_bus.OR(*d, *d0, *d1, *d2);
    this->update_cycles(5);  // 3 reads, 1 op, 1 write
}

void SCAMP5::OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3) {
    // d := d0 OR d1 OR d2 OR d3
    this->pe->local_read_bus.OR(*d, *d0, *d1, *d2, *d3);
    this->update_cycles(6);  // 4 reads, 1 op, 1 write
}

void SCAMP5::NOT(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0) {
    // d := NOT d0
    this->pe->local_read_bus.NOT(*d, *d0);
    this->update_cycles(3); // 1 read, 1 op, 1 write
}

void SCAMP5::NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1) {
    // d := NOT(d0 OR d1)
    this->pe->local_read_bus.NOR(*d, *d0, *d1);
    this->update_cycles(5);  // 2 reads, 2 op, 1 write
}

void SCAMP5::NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2) {
    // d := NOT(d0 OR d1 OR d2)
    this->pe->local_read_bus.NOR(*d, *d0, *d1, *d2);
    this->update_cycles(6);  // 3 reads, 2 op, 1 write
}

void SCAMP5::NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    this->pe->local_read_bus.NOR(*d, *d0, *d1, *d2, *d3);
    this->update_cycles(7); // 4 reads, 2 op, 1 write
}

void SCAMP5::NOT(const std::shared_ptr<DREG>& Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
    this->update_cycles(4); // 2 reads, 1 op, 1 write
}

void SCAMP5::OR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx) {
    // Rl := Rl OR Rx
    this->OR(Rl, Rl, Rx);
}

void SCAMP5::NOR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx) {
    // Rl := Rl NOR Rx
    this->NOR(Rl, Rl, Rx);
}

void SCAMP5::AND(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->NOR(Ra, RF, RP);
}

void SCAMP5::NAND(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->OR(Ra, RF, RP);
}

void SCAMP5::ANDX(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rb, const std::shared_ptr<DREG>& Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, RF, Rb);
}

void SCAMP5::NANDX(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rb, const std::shared_ptr<DREG>& Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, RF, Rb);
}

void SCAMP5::IMP(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry) {
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

void SCAMP5::NIMP(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(RF, Ry);
    this->NOR(RS, Rx, RF);
}

void SCAMP5::XOR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(RF, Ry);
    this->NOR(Rl, Rx, RF);
    this->NOT(RF, Rx);
    this->NOR(Rx, Ry, RF);
    this->OR(Rl, Rx);
}

void SCAMP5::WHERE(const std::shared_ptr<DREG>& d) {
    // FLAG := d.
    this->FLAG->write(d->read(), FLAG->get_mask());
    this->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::WHERE(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1) {
    // FLAG := d0 OR d1.
    this->OR(intermediate_d, d0, d1);
    this->FLAG->write(intermediate_d->read(), FLAG->get_mask());
    this->update_cycles(1);  // 1 write
}

void SCAMP5::WHERE(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2) {
    // FLAG := d0 OR d1 OR d2.
    this->OR(intermediate_d, d0, d1, d2);
    this->FLAG->write(intermediate_d->read(), FLAG->get_mask());
    this->update_cycles(1);  // 1 write
}

void SCAMP5::ALL() {
    // FLAG := 1, same as all.
    this->FLAG->write(1, FLAG->get_mask());
    this->update_cycles(1);  // 1 write
}

void SCAMP5::SET(const std::shared_ptr<DREG>& d0) {
    // d0 := 1
    d0->write(1, d0->get_mask());
    this->update_cycles(1);  // 1 write
}

void SCAMP5::SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1) {
    // d0, d1 := 1
    d0->write(1, d0->get_mask());
    d1->write(1, d1->get_mask());
    this->update_cycles(2);  // 2 writes
}

void SCAMP5::SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2) {
    // 	d0, d1, d2 := 1
    d0->write(1, d0->get_mask());
    d1->write(1, d1->get_mask());
    d2->write(1, d2->get_mask());
    this->update_cycles(3);  // 3 writes
}

void SCAMP5::SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3) {
    // d0, d1, d2, d3 := 1
    d0->write(1, d0->get_mask());
    d1->write(1, d1->get_mask());
    d2->write(1, d2->get_mask());
    d3->write(1, d3->get_mask());
    this->update_cycles(4);  // 4 writes
}

void SCAMP5::CLR(const std::shared_ptr<DREG>& d0) {
    // d0 := 0
    d0->write(0, d0->get_mask());
    this->update_cycles(1);  // 1 write
}

void SCAMP5::CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1) {
    // d0, d1 := 0
    d0->write(0, d0->get_mask());
    d1->write(0, d1->get_mask());
    this->update_cycles(2);  // 2 writes
}

void SCAMP5::CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2) {
    // d0, d1, d2 := 0
    d0->write(0, d0->get_mask());
    d1->write(0, d1->get_mask());
    d2->write(0, d2->get_mask());
    this->update_cycles(3);  // 3 writes
}

void SCAMP5::CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3) {
    // 	d0, d1, d2, d3 := 0
    d0->write(0, d0->get_mask());
    d1->write(0, d1->get_mask());
    d2->write(0, d2->get_mask());
    d3->write(0, d3->get_mask());
    this->update_cycles(4);  // 4 writes
}

void SCAMP5::MOV(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0) {
    // d := d0
    this->pe->local_read_bus.MOV(*d, *d0);
    this->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::MUX(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry, const std::shared_ptr<DREG>& Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    this->pe->local_read_bus.MUX(*Rl, *Rx, *Ry, *Rz);
    this->update_cycles(4);  // 3 reads, 1 write, some op?
}

void SCAMP5::CLR_IF(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    this->pe->local_read_bus.CLR_IF(*Rl, *Rx);
    this->update_cycles(2);  // 1 read, up to 1 write, some op for if?
}

void SCAMP5::REFRESH(const std::shared_ptr<DREG>& Rl) {
    // 	refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds)
    // without any operations
}

void SCAMP5::DNEWS0(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 0 from the edge
    DREG east = DREG(this->rows_, this->cols_);
    DREG north = DREG(this->rows_, this->cols_);
    DREG west = DREG(this->rows_, this->cols_);
    DREG south = DREG(this->rows_, this->cols_);

    this->pe->local_read_bus.get_east(east, *d0, 1, 0, this->origin_);
    this->pe->local_read_bus.get_north(north, *d0, 1, 0, this->origin_);
    this->pe->local_read_bus.get_west(west, *d0, 1, 0, this->origin_);
    this->pe->local_read_bus.get_south(south, *d0, 1, 0, this->origin_);

    std::shared_ptr<DREG> east_ptr = std::make_shared<DREG>(east);
    std::shared_ptr<DREG> west_ptr = std::make_shared<DREG>(west);
    std::shared_ptr<DREG> north_ptr = std::make_shared<DREG>(north);
    std::shared_ptr<DREG> south_ptr = std::make_shared<DREG>(south);

    AND(east_ptr, east_ptr, RE);
    AND(north_ptr, north_ptr, RN);
    AND(west_ptr, west_ptr, RW);
    AND(south_ptr, south_ptr, RS);

    OR(d, east_ptr, north_ptr, south_ptr, west_ptr);
}

void SCAMP5::DNEWS1(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 1 from the edge
    DREG east = DREG(this->rows_, this->cols_);
    DREG north = DREG(this->rows_, this->cols_);
    DREG west = DREG(this->rows_, this->cols_);
    DREG south = DREG(this->rows_, this->cols_);

    this->pe->local_read_bus.get_east(east, *d0, 1, 1, this->origin_);
    this->pe->local_read_bus.get_north(north, *d0, 1, 1, this->origin_);
    this->pe->local_read_bus.get_west(west, *d0, 1, 1, this->origin_);
    this->pe->local_read_bus.get_south(south, *d0, 1, 1, this->origin_);

    std::shared_ptr<DREG> east_ptr = std::make_shared<DREG>(east);
    std::shared_ptr<DREG> west_ptr = std::make_shared<DREG>(west);
    std::shared_ptr<DREG> north_ptr = std::make_shared<DREG>(north);
    std::shared_ptr<DREG> south_ptr = std::make_shared<DREG>(south);

    AND(east_ptr, east_ptr, RE);
    AND(north_ptr, north_ptr, RN);
    AND(west_ptr, west_ptr, RW);
    AND(south_ptr, south_ptr, RS);

    OR(d, east_ptr, north_ptr, south_ptr, west_ptr);
}

void SCAMP5::DNEWS(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, int dir, bool boundary) {
    // digital neighbour OR, Ra := Rx_dir1 OR Rx_dir2 ...; (also modify R1 R2 R3
    // R4).
    this->CLR(RS, RW, RN, RE);
    // set multiple DREG& (upto 4) can be done via one icw
    if(dir & south) {
        this->SET(RS);
    }
    if(dir & west) {
        this->SET(RW);
    }
    if(dir & north) {
        this->SET(RN);
    }
    if(dir & east) {
        this->SET(RE);
    }

    if(boundary) {
        this->DNEWS1(Ra, Rx);
    } else {
        this->DNEWS0(Ra, Rx);
    }
}

void SCAMP5::PROP0() {
    // async-propagation on R12, masked by R0
}

void SCAMP5::PROP1() {
    // async-propagation on R12, masked by R0 when boundaries are considered '1'
}

void SCAMP5::scamp5_get_image(const std::shared_ptr<AREG>& yf, const std::shared_ptr<AREG>& yh, int gain) {
    // put the exposure result in *PIX to AREGs and reset *PIX
    // yf	full range [-128,127]
    // yh	half range [0,127]
    // gain	(optional) gain [1,5]
    // get_image(A, B, 0|1)
    // TODO improve capture rate. Multithreading needed?

#ifdef USE_RUNTIME_CHECKS
    if(gain < 1 || gain > 5) {
        std::cout << "[Warning] Gain should be in [1,5]" << std::endl;
    }
#endif
    all();
    this->pe->get_pixel()->read(*PIX);
    bus(NEWS, PIX);
    rpix();
    this->pe->get_pixel()->read(*PIX);
    bus(yf, NEWS, PIX);
    bus(NEWS, yf);

    for(int i = 1; i < gain; i++) {
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
    this->pe->get_pixel()->read(*PIX);
    bus(NEWS, yh, yf, PIX);
    bus(yf, NEWS);
}

void SCAMP5::scamp5_in(const std::shared_ptr<AREG>& areg, int16_t value, std::shared_ptr<AREG>& temp) {
    // load an analog value to the AREG with error&noise correction
    // TODO noise
    IN->write(value);
    this->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_in(const std::shared_ptr<AREG>& areg, int16_t value) {
    // load an analog value to the AREG with error&noise correction
    scamp5_in(areg, value, NEWS);
}

void SCAMP5::scamp5_load_in(const std::shared_ptr<AREG>& areg, int8_t value, std::shared_ptr<AREG>& temp) {
    // load a analog value to the AREG plane without error&noise correction
    // TODO noise
    IN->write(value);
    this->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_load_in(const std::shared_ptr<AREG>& areg, int8_t value) {
    // load a analog value to the AREG plane without error&noise correction
    scamp5_load_in(areg, value, NEWS);
}

void SCAMP5::scamp5_load_in(int8_t value) {
    // load a analog value to IN without error&noise correction
    // TODO noise
    IN->write(value);
    this->update_cycles(1);
}

void SCAMP5::scamp5_load_dac(const std::shared_ptr<AREG>& areg, uint16_t value, std::shared_ptr<AREG>& temp) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	temporary kernel register to be used in the function
    IN->write(value);
    this->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_load_dac(const std::shared_ptr<AREG>& areg, uint16_t value) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	= NEWS
    scamp5_load_dac(areg, value, NEWS);
}

void SCAMP5::scamp5_load_dac(uint16_t value) {
    // load an analog value to IN using a raw DAC value
    // TODO What is with the range of values here. Why can some registers hold a
    // much larger range?
    IN->write(value);
    this->update_cycles(1);
}

void SCAMP5::scamp5_shift(const std::shared_ptr<AREG>& areg, int h, int v) {
    // shift an AREG image
    // this->pe->analogue_bus.

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
    //        const std::shared_ptr<AREG>& direction = *(h > 0 ? EAST : WEST);
    //        const std::shared_ptr<AREG>& end = (h % 2 == 0 ? direction : *NEWS);
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

void SCAMP5::scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations) {
    scamp5_diffuse(target, iterations, true, true, NEWS);
}

void SCAMP5::scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical) {
    scamp5_diffuse(target, iterations, vertical, true, NEWS);
}

void SCAMP5::scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical, bool horizontal) {
    scamp5_diffuse(target, iterations, vertical, horizontal, NEWS);
}

void SCAMP5::scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical, bool horizontal, std::shared_ptr<AREG>& t0) {
    // diffuse an AREG image
    // TODO is this the same as Gaussian blur?
    void (SCAMP5::*blur_func)(const std::shared_ptr<AREG>& , const std::shared_ptr<AREG>& );
    if(horizontal && vertical) {
        blur_func = &SCAMP5::blur;
    } else if(horizontal) {
        blur_func = &SCAMP5::blurh;
    } else {
        blur_func = &SCAMP5::blurv;
    }

    (*this.*blur_func)(t0, target);
    (*this.*blur_func)(target, t0);
    for(int i = 1; i < iterations; i++) {
        (*this.*blur_func)(t0, target);
        (*this.*blur_func)(target, t0);
    }
}

uint8_t SCAMP5::scamp5_read_areg(const std::shared_ptr<AREG>& areg, uint8_t r, uint8_t c) {
    // read a single pixel
    // TODO check that the value is properly mapped to uint8_t from CV_16U
    return areg->read().at<uint8_t>(r, c);
}

uint32_t SCAMP5::scamp5_global_sum_16(const std::shared_ptr<AREG>& areg, uint8_t *result16v) {
    // get the AREG sum level using a set of 4x4 sparse grid.
    // When result16v is a NULL pointer, the function will return sum of the
    // data. The result is not equal to the actual sum of all pixels of the AREG
    // in the area, but it is proportional to the actual sum. This function
    // takes roughly 14 usec (on the scamp5 device) to execute_instructions.
    // TODO should not be exact
    uint32_t sum = 0;
    int buf_index = 0;
    int cs = this->cols_ / 4;
    int rs = this->rows_ / 4;
    for(int col = 0; col < this->cols_; col += cs) {
        for(int row = 0; row < this->rows_; row += rs) {
            // TODO double check width and height
            int val = cv::sum(
                areg->read()(cv::Rect(col, row, col + cs, row + rs)))[0];
            if(result16v == nullptr) {
                sum += val;
            } else {
                result16v[buf_index++] = val;
            }
        }
    }
    this->update_cycles(140); // 14usec at 10MHz. TODO make flexible
    return sum;
}

uint32_t SCAMP5::scamp5_global_sum_64(const std::shared_ptr<AREG>& areg, uint8_t *result64v) {
    // get the AREG sum level with a better resolution
    // This function achieves similar functionally as the "global_sum_16"
    // version, but the grid used is 8x8. As a consequence, the result has
    // higher resolution but it takes longer to execute_instructions (roughly 28 usec on the
    // scamp5).
    uint32_t sum = 0;
    int buf_index = 0;
    int cs = this->cols_ / 8;
    int rs = this->rows_ / 8;
    for(int col = 0; col < this->cols_; col += cs) {
        for(int row = 0; row < this->rows_; row += rs) {
            int val = cv::sum(
                areg->read()(cv::Rect(col, row, col + cs, row + rs)))[0];
            if(result64v == nullptr) {
                sum += val;
            } else {
                result64v[buf_index++] = val;
            }
        }
    }
    this->update_cycles(280); // 28usec at 10MHz. TODO improve
    return sum;
}

uint8_t SCAMP5::scamp5_global_sum_fast(const std::shared_ptr<AREG>& areg) {
    // get approximate sum level of the whole AREG plane
    // TODO should be approximate sum not exact. Also need to abstract away cv call
    this->update_cycles(70); // TODO improve
    return cv::sum(areg->read())[0];
}

uint8_t SCAMP5::scamp5_global_sum_sparse(const std::shared_ptr<AREG>& areg, uint8_t r, uint8_t c,
                                         uint8_t rx, uint8_t cx) {
    // get sum level of the pixels selected using a pattern
    // This result is less probable to saturate because it only counts a quarter
    // of the pixels in the AREG plane (by default)
    unsigned int r_mask = ((~rx) & (this->rows_ - 1));
    unsigned int c_mask = ((~cx) & (this->cols_ - 1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    uint8_t sum = 0;

    for(unsigned int row_index = 0; row_index < this->cols_; row_index++) {
        for(unsigned int col_index = 0; col_index < this->rows_; col_index++) {
            if(((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                sum += areg->read().at<uint8_t>(row_index, col_index);
            }
        }
    }
    this->update_cycles(70); // TODO improve
    return sum;
}

void SCAMP5::scamp5_shift(const std::shared_ptr<DREG>& dreg, int h, int v, const int boundary) {
    // shift a DREG image
}

int SCAMP5::scamp5_global_or(const std::shared_ptr<DREG>& dreg, uint8_t r, uint8_t c, uint8_t rx,
                             uint8_t cx) {
    // get OR result of all pixels in a DREG plane. 0 if all pixel is 0,
    // non-zero otherwise. The default mask pattern parameters gives the whole
    // image. scamp5_load_pattern can be used to work out the correct parameter
    // for a desired mask pattern,
    // TODO abstraction
    unsigned int r_mask = ((~rx) & (this->rows_ - 1));
    unsigned int c_mask = ((~cx) & (this->cols_ - 1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    uint8_t val = 0;

    for(unsigned int row_index = 0; row_index < this->cols_; row_index++) {
        for(unsigned int col_index = 0; col_index < this->rows_; col_index++) {
            if(((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                val |= dreg->read().at<uint8_t>(row_index, col_index);
            }
        }
    }
    this->update_cycles(70); // TODO improve
    return val;
}

int SCAMP5::scamp5_global_count(const std::shared_ptr<DREG>& dreg, const std::shared_ptr<AREG>& temp, int options) {
    // get an estimation of the number of '1's in a DREG plane
    // Internally this function converts the DREG to AREG using suitable analog
    // levels to represent '0's and '1's. Then a AREG global sum is done and the
    // result is uniformed into [0,4095].
    // TODO options and estimation
    dreg->read().convertTo(temp->read(), 255, 0);
    int total = cv::sum(temp->read())[0];
    double min_val, max_val;
    cv::minMaxLoc(temp->read(), &min_val, &max_val);
    return utility::normalise(total, min_val, max_val, 0, 4096);
}

void SCAMP5::scamp5_flood(const std::shared_ptr<DREG>& dreg_target, const std::shared_ptr<DREG>& dreg_mask, int boundary,
                          int iterations, bool use_local) {
    // flooding a DREG image with '1's from the '1's in the DREG image
    // dreg_target	the DREG to be flooded
    // dreg_mask	the control DREG to inhibit the flood
    // boundary	    boundary value
    // iterations	(optional) number of iterations
    // use_local	(optional) use pre-exist R1-R4 to control the flood
    // direction The target DREG will be flooded by '1's except those areas that
    // are closed by '0's in the control register. To increase the travelling
    // distance of the flood, the number of iteration should be increased.

    // dreg_mask can contain many different seeds. Each one is called
    // individually to start the flood fill

    // TODO ensure mask is correct size after floodfill operation

    std::vector<cv::Point> seeds;  // locations of white pixels in the original image
    cv::findNonZero(dreg_target->read(), seeds);

    uint8_t fillValue = 1;

    // Add additional pixel to each side
    cv::Mat mask;
    cv::copyMakeBorder(dreg_mask->read(), mask, 1, 1, 1, 1,
                       cv::BORDER_REPLICATE);

    dreg_mask->read() = 1 - dreg_mask->read();

    for(auto &seed: seeds) {
        cv::floodFill(dreg_mask->read(), mask, seed, cv::Scalar(1), nullptr,
                      cv::Scalar(0), cv::Scalar(1), 4);
    }
}

void SCAMP5::scamp5_load_point(const std::shared_ptr<DREG>& dr, uint8_t r, uint8_t c) {
    // set a single pixel on a DREG image to 1, the rest to 0
    dr->read().setTo(0);
    dr->read().at<uint8_t>(r, c) = 1;
}

void SCAMP5::scamp5_load_rect(const std::shared_ptr<DREG>& dr, uint8_t r0, uint8_t c0, uint8_t r1,
                              uint8_t c1) {
    // set a rectangle area on a DREG image to 1, the rest to 0
    // r0	pixel row index of the top right corner
    // c0	pixel column index of the top right corner
    // r1	pixel row index of the bottom left corner
    // c1	pixel column index of the bottom left corner

    // TODO In SCAMP5, (0,0) is in the top right. Need to convert this
    // everywhere OpenCV has (0,0) at top left.

    // TODO check
#ifndef NDEBUG
    assert(r0 - r1 > 0);
    assert(r0 - r1 > 0);
#endif
    int width = c0 - c1;
    int height = r0 - r1;
    dr->clear();
    dr->read()(cv::Rect(r0, c1, width, height)).setTo(1);
}

void SCAMP5::scamp5_load_pattern(const std::shared_ptr<DREG>& dr, uint8_t r, uint8_t c, uint8_t rx,
                                 uint8_t cx) {
    // set those pixels with matching address to 1, the rest to 0
    // To mask out certain bits in the column/row address allow a match to occur
    // periodically. For example, set mask to 192 (11000000b) makes the four
    // following address match the value 3 (00000011b): 3(00000011b),
    // 67(01000011b), 131(10000011b) and 195(11000011b).
    // TODO abstraction
    dr->clear();

    unsigned int r_mask = ((~rx) * (this->rows_ - 1));
    unsigned int c_mask = ((~cx) * (this->cols_ - 1));

    unsigned int r_f = r * r_mask;
    unsigned int c_f = c * c_mask;

    for(unsigned int row_index = 0; row_index < this->cols_; row_index++) {
        for(unsigned int col_index = 0; col_index < this->rows_; col_index++) {
            if(((row_index * r_mask) == r_f) && ((col_index * c_mask) == c_f)) {
                dr->read().at<uint8_t>(row_index, col_index) = 1;
            }
        }
    }
}

void SCAMP5::scamp5_select_point(uint8_t r, uint8_t c) {
    // same as scamp5_load_point, but put the point in SELECT
    scamp5_load_point(SELECT, r, c);
}

void SCAMP5::scamp5_select_rect(uint8_t r0, uint8_t c0, uint8_t r1,
                                uint8_t c1) {
    // same as scamp5_load_rect, but put the rectangle in RECT (also modify
    // SELECT)
    // TODO why is SELECT modified?
    scamp5_load_rect(RECT, r0, c0, r1, c1);
}

void SCAMP5::scamp5_select_pattern(uint8_t r, uint8_t c, uint8_t rx,
                                   uint8_t cx) {
    // same as scamp5_load_pattern, but put the pattern in SELECT
    // TODO check
    scamp5_load_pattern(SELECT, r, c, rx, cx);
}

void SCAMP5::scamp5_select_col(uint8_t c) {
    // select column
    for(unsigned int row_index = 0; row_index < this->cols_; row_index++) {
        for(unsigned int col_index = 0; col_index < this->rows_; col_index++) {
            if(col_index == c) {
                SELECT->read().at<uint8_t>(row_index, col_index) = 1;
            }
        }
    }
}

void SCAMP5::scamp5_select_row(uint8_t r) {
    // select row
    for(unsigned int row_index = 0; row_index < this->cols_; row_index++) {
        for(unsigned int col_index = 0; col_index < this->rows_; col_index++) {
            if(row_index == r) {
                SELECT->read().at<uint8_t>(row_index, col_index) = 1;
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

void SCAMP5::scamp5_draw_begin(const std::shared_ptr<DREG>& dr) {
    // targeting a DREG image to a series of drawing operations
    scratch = dr;
}

void SCAMP5::scamp5_draw_end() {
    // end the drawing operations
    scratch = nullptr;
}

void SCAMP5::scamp5_draw_pixel(uint8_t r, uint8_t c) {
    // draw a point, wrap around if it's outside the border
    scratch->read().at<uint8_t>(r % this->rows_, c % this->cols_) = 1;
}

bool SCAMP5::scamp5_draw_point(int r, int c) {
    // draw a point when its position is within the image
    // returns whether the point is inside the image and drawn
    if(r >= this->rows_ || c >= this->cols_) {
        return false;
    }
    scratch->read().at<uint8_t>(r, c) = 1;
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
    scratch->read()(cv::Rect(c0, r0, width, height)).setTo(1);
}

void SCAMP5::scamp5_draw_line(int r0, int c0, int r1, int c1, bool repeat) {
    // draw a line
    // r0 - starting point row coordinate
    // c0 - starting point column coordinate
    // r1 - finishing point row coordinate
    // c1 - finishing point column coordinate
    // repeat - whether to wrap around when point goes outside the image
    // TODO wrap around
    cv::line(scratch->read(), {r0, c0}, {r1, c1}, 1);
}

void SCAMP5::scamp5_draw_circle(int x0, int y0, int radius, bool repeat) {
    // draw a non-filled circle - uses Bresenham's Midpoint Circle algorithm
    // repeat is for controlling wrap around
    // TODO need to use scamp5_load_point like existing sim
#ifdef USE_RUNTIME_CHECKS
    if(scratch == nullptr) {
        std::cerr << "Drawing register has not been set" << std::endl;
    }
#endif

    int f = 1 - radius;
    int ddf_x = 1;
    int ddf_y = -2 * radius;
    int x = 0;
    int y = radius;

    scratch->read().at<uint8_t>(y0 + radius, x0) = 1;
    scratch->read().at<uint8_t>(y0 - radius, x0) = 1;
    scratch->read().at<uint8_t>(y0, x0 + radius) = 1;
    scratch->read().at<uint8_t>(y0, x0 - radius) = 1;

    while(x < y) {
        if(f >= 0) {
            y -= 1;
            ddf_y += 2;
            f += ddf_y;
        }

        x += 1;
        ddf_x += 2;
        f += ddf_x;

        scratch->read().at<uint8_t>(y0 + y, x0 + x) = 1;
        scratch->read().at<uint8_t>(y0 + y, x0 - x) = 1;
        scratch->read().at<uint8_t>(y0 - y, x0 + x) = 1;
        scratch->read().at<uint8_t>(y0 - y, x0 - x) = 1;
        scratch->read().at<uint8_t>(y0 + x, x0 + y) = 1;
        scratch->read().at<uint8_t>(y0 + x, x0 - y) = 1;
        scratch->read().at<uint8_t>(y0 - x, x0 + y) = 1;
        scratch->read().at<uint8_t>(y0 - x, x0 - y) = 1;
    }
}

void SCAMP5::scamp5_draw_negate() {
    // do a binary inversion of the DREG image.
    // TODO abstraction
    scratch->read() = 1 - scratch->read();
}

// Image Readout

void SCAMP5::scamp5_scan_areg(const std::shared_ptr<AREG>& areg, uint8_t *buffer, uint8_t r0,
                              uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs,
                              uint8_t cs) {
    // scan a customized grid of pixels in an AREG image
    // Note, the result image is stored in column-major format, starting from
    // top right. i.e. "buffer[3]" is the pixel the on 1st column right, 4th row
    // down. This applies to all "scamp5_scan_areg_*" series functions.
    this->pe->analogue_bus.scan(buffer, *areg, r0, c0, r1, c1, rs, cs, this->origin_);
}

void SCAMP5::scamp5_scan_areg_8x8(const std::shared_ptr<AREG>& areg, uint8_t *result8x8) {
    // scan a 8x8 grid of pixels in an AREG image
    // This function is slightly faster and more accurate than scamp5_scan_areg.
    // TODO currently assuming this takes beginning of each cell in 8x8 grid.
    // Might be center instead
    // TODO check RowMajor/ColMajor stuff
    int buf_index = 0;
    int cs = this->cols_ / 8;
    int rs = this->rows_ / 8;
    for(int col = 0; col < this->cols_; col += cs) {
        for(int row = 0; row < this->rows_; row += rs) {
            result8x8[buf_index++] = areg->read().at<uint8_t>(row, col);
        }
    }
}

void SCAMP5::scamp5_scan_areg_mean_8x8(const std::shared_ptr<AREG>& areg, uint8_t *result8x8) {
    // divide the AREG image into 8x8 square blocks, and get the average of each
    // block result8x8 - pointer to a buffer to store the results
    this->pe->analogue_bus.blocked_average(result8x8, *areg, 8, this->origin_);
}

void SCAMP5::scamp5_scan_dreg(const std::shared_ptr<DREG>& dreg, uint8_t *mem, uint8_t r0,
                              uint8_t r1) {
    // scan DREG image, store the result in a buffer
    // mem - pointer to a buffer
    // r0 - first row index
    // r1 - last row index
    // The size of the buffer need to be a least 32 times the number of rows to
    // scan. Thus, a full DREG image requires a buffer of 8192 bytes.
    // TODO check if it should be (row, col) or (col, row)
    // TODO check impl
    int buf_index = 0;
    for(uint32_t row_index = r0; row_index <= r1; row_index++) {
        // Read 8 values at a time to make up a byte
        for(int col_index = 0; col_index < this->cols_; col_index += 8) {
            uint8_t b0 = dreg->read().at<uint8_t>(row_index, col_index);
            uint8_t b1 = dreg->read().at<uint8_t>(row_index, col_index + 1);
            uint8_t b2 = dreg->read().at<uint8_t>(row_index, col_index + 2);
            uint8_t b3 = dreg->read().at<uint8_t>(row_index, col_index + 3);
            uint8_t b4 = dreg->read().at<uint8_t>(row_index, col_index + 4);
            uint8_t b5 = dreg->read().at<uint8_t>(row_index, col_index + 5);
            uint8_t b6 = dreg->read().at<uint8_t>(row_index, col_index + 6);
            uint8_t b7 = dreg->read().at<uint8_t>(row_index, col_index + 7);
            uint8_t value = (b0 << 7) | (b1 << 6) | (b2 << 5) | (b3 << 4) |
                            (b4 << 3) | (b5 << 2) | (b6 << 1) | (b7 << 0);
            mem[buf_index++] = value;
        }
    }
}

void SCAMP5::scamp5_scan_events(const std::shared_ptr<DREG>& dreg, uint8_t *mem, uint16_t max_num,
                                uint8_t h_dir, uint8_t v_dir) {
    // scan the coordinates of all '1's in a DREG image and store the result in
    // a buffer mem - pointer to a buffer of (max_num*2) bytes max_num - maximum
    // number of events h_dir - horizontal order of scanning, '0' means from
    // column 0 to column 255 v_dir - vertical order of scanning, '0' means from
    // row 0 to row 255 Up-to 4000 events scan be scanned. Note, the execution
    // time of this function is proportional to max_num. In the result, the
    // first occurrence of (0,0) indicates the end of effective events. writes
    // to mem as (col, row)
    // TODO Row/Col confusion. Make consistent
    // FIXME with proper setting of (0, 0)
    // TODO does not take scanning direction into account. Rewrite
    std::vector<cv::Point> locations;  // output, locations of non-zero pixels
    cv::findNonZero(dreg->read(), locations);
    int buf_index = 0;
    for(auto &p: locations) {
        if(buf_index == 2 * max_num)
            break;
        mem[buf_index++] = p.x;
        mem[buf_index++] = p.y;
    }
}

void SCAMP5::scamp5_scan_events(const std::shared_ptr<DREG>& dreg, uint8_t *buffer, uint16_t max_num,
                                uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1,
                                uint8_t rs, uint8_t cs) {
    // assuming 0,0 in top left
    int buf_index = 0;
    for(int col = c0; col < c1; col += cs) {
        for(int row = r0; row < r1; row += rs) {
            if(dreg->read().at<uint8_t>(row, col) > 0) {
                if(buf_index == 2 * max_num)
                    return;
                buffer[buf_index++] = col;
                buffer[buf_index++] = row;
            }
        }
    }
}

void SCAMP5::scamp5_scan_boundingbox(const std::shared_ptr<DREG>& dr, uint8_t *vec4v) {
    // scan the bounding box coordinates of '1's in a DREG image
    // The coordinates are two points: the top-right and the bottom-left corners
    // of the axis-aligned bounding box (AABB) of '1's in the DREG. vec4v -
    // pointer to a buffer of 4 byte
}

// Simulator specific

//todo remove
void SCAMP5::print_stats() {
    // TODO move
#ifdef TRACK_STATISTICS
    this->update_static(); //move
    Architecture::print_stats();
//    json j;
//    j["Total number of cycles"] = counter->get_cycles();
//    j["Equivalent in seconds"] = counter->to_seconds(config_.clock_rate_);
//
//    // this.print_stats(counter);
//    this->write_stats(*counter, j);
//    std::cout << std::setw(2) << j << std::endl;
//    std::ofstream file_out;
//    std::cout << std::filesystem::current_path().string() << std::endl;
//    file_out.open(std::filesystem::current_path().string() + "/output.json");
//    file_out << std::setw(2) << j;
//    file_out.close();
#endif
#ifndef TRACK_STATISTICS
    std::cerr << "Simulator has not been compiled with statistic tracking support. Recompile with -DTRACK_STATISTICS=ON" << std::endl;
#endif
}

// todo move into base class
rttr::variant SCAMP5::components_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<Component>> components;
    try {
        for (auto& [_, value] : j.items()) {
            std::string name = value["_name"];
            std::string component = value["_component"];
            std::shared_ptr<Component> instance = Parser::get_instance().create_instance(component, value).get_value<std::shared_ptr<Component>>();
            components[name] = instance;
        }
        return rttr::variant(components);
    } catch (json::type_error&) {
        std::cerr << "[Warning] Could not parse component" << std::endl;
    } catch (json::parse_error&) {
        std::cerr << "[Warning] Could not parse component" << std::endl;
    }

    return rttr::variant();
}

rttr::variant SCAMP5::config_converter(json& j) {
    return Parser::get_instance().create_instance("Config", j);
}

void SCAMP5::set_rows(int rows) {
    this->rows_ = rows;
}

void SCAMP5::set_cols(int cols) {
    this->cols_ = cols;
}

void SCAMP5::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void SCAMP5::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void SCAMP5::set_origin(Origin origin) {
    this->origin_ = origin;
}

void SCAMP5::set_config(std::shared_ptr<Config> config) {
    this->config_ = std::move(config);
}

void SCAMP5::set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components) {
    this->components_ = std::move(components);
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::enumeration<news_t>("news_t") (
        value("north", news_t::north),
        value("east", news_t::east),
        value("south", news_t::south),
        value("west", news_t::west)
    );

    registration::class_<SCAMP5>("SCAMP5")
        .constructor()
        .method("init", &SCAMP5::init)
        .method("config_converter", &SCAMP5::config_converter)
        .method("components_converter", &SCAMP5::components_converter)
        .method("set_rows", &SCAMP5::set_rows)
        .method("set_cols", &SCAMP5::set_cols)
        .method("set_row_stride", &SCAMP5::set_row_stride)
        .method("set_col_stride", &SCAMP5::set_col_stride)
        .method("set_origin", &SCAMP5::set_origin)
        .method("set_config", &SCAMP5::set_config)
        .method("set_components", &SCAMP5::set_components)
        .method("nop", &SCAMP5::nop)
        .method("rpix", &SCAMP5::rpix)
        .method("get_image", select_overload<void(const std::shared_ptr<AREG>&)> (&SCAMP5::get_image))
        .method("get_image", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::get_image))
        .method("respix", select_overload<void()>(&SCAMP5::respix))
        .method("respix", select_overload<void(const std::shared_ptr<AREG>&)>(&SCAMP5::respix))
        .method("getpix", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::getpix))
        .method("getpix", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::getpix))
        .method("bus", select_overload<void(const std::shared_ptr<AREG>&)>(&SCAMP5::bus))
        .method("bus", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus))
        .method("bus", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus))
        .method("bus", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus))
        .method("bus", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus))
        .method("bus2", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus2))
        .method("bus2", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus2))
        .method("bus3", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::bus3))
        .method("where", select_overload<void(const std::shared_ptr<AREG>&)>(&SCAMP5::where))
        .method("where", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::where))
        .method("where", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::where))
        .method("all", &SCAMP5::all)
        .method("mov", &SCAMP5::mov)
        .method("res", select_overload<void(const std::shared_ptr<AREG>&)>(&SCAMP5::res))
        .method("res", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::res))
        .method("add", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::add))
        .method("add", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::add))
        .method("sub", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::sub))
        .method("neg", &SCAMP5::neg)
        .method("abs", &SCAMP5::abs)
        .method("div", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::div))
        .method("div", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::div))
        .method("diva", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::diva))
        .method("divq", select_overload<void(const std::shared_ptr<AREG>&, const std::shared_ptr<AREG>&)>(&SCAMP5::divq))
        .method("movx", &SCAMP5::movx)
        .method("mov2x", &SCAMP5::mov2x)
        .method("addx", &SCAMP5::addx)
        .method("add2x", &SCAMP5::add2x)
        .method("subx", &SCAMP5::subx)
        .method("sub2x", &SCAMP5::sub2x)
        .method("blurset", &SCAMP5::blurset)
        .method("blur", &SCAMP5::blur)
        .method("blurh", &SCAMP5::blurh)
        .method("blurv", &SCAMP5::blurv)
        .method("gauss", &SCAMP5::gauss)(default_arguments(3))
        .method("gaussh", &SCAMP5::gaussh)(default_arguments(3))
        .method("gaussv", &SCAMP5::gaussv)(default_arguments(3))
        .method("newsblur", &SCAMP5::newsblur)(default_arguments(1))
        .method("newsblurh", &SCAMP5::newsblurh)(default_arguments(1))
        .method("newsblurv", &SCAMP5::newsblurv)(default_arguments(1))
        .method("OR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::OR))
        .method("OR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::OR))
        .method("OR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::OR))
        .method("NOT", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::NOT))
        .method("NOR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(const std::shared_ptr<DREG>&)>(&SCAMP5::NOT))
        .method("OR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::OR))
        .method("NOR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::NOR))
        .method("AND", &SCAMP5::AND)
        .method("NAND", &SCAMP5::NAND)
        .method("ANDX", &SCAMP5::ANDX)
        .method("NANDX", &SCAMP5::NANDX)
        .method("IMP", &SCAMP5::IMP)
        .method("NIMP", &SCAMP5::NIMP)
        .method("XOR", &SCAMP5::XOR)
        .method("WHERE", select_overload<void(const std::shared_ptr<DREG>&)>(&SCAMP5::WHERE))
        .method("WHERE", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::WHERE))
        .method("WHERE", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::WHERE))
        .method("ALL", &SCAMP5::ALL)
        .method("SET", select_overload<void(const std::shared_ptr<DREG>&)>(&SCAMP5::SET))
        .method("SET", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::SET))
        .method("SET", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::SET))
        .method("SET", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::SET))
        .method("CLR", select_overload<void(const std::shared_ptr<DREG>&)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&, const std::shared_ptr<DREG>&)>(&SCAMP5::CLR))
        .method("MOV", &SCAMP5::MOV)
        .method("MUX", &SCAMP5::MUX)
        .method("CLR_IF", &SCAMP5::CLR_IF)
        .method("REFRESH", &SCAMP5::REFRESH)
        .method("DNEWS0", &SCAMP5::DNEWS0)
        .method("DNEWS1", &SCAMP5::DNEWS1)
        .method("DNEWS", &SCAMP5::DNEWS)
        .method("PROP0", &SCAMP5::PROP0)
        .method("PROP1", &SCAMP5::PROP1)
        .method("scamp5_get_image", &SCAMP5::scamp5_get_image)(default_arguments(1))
        .method("scamp5_in", select_overload<void(const std::shared_ptr<AREG>&, int16_t)>(&SCAMP5::scamp5_in))
        .method("scamp5_in", select_overload<void(const std::shared_ptr<AREG>&, int16_t, std::shared_ptr<AREG>&)>(&SCAMP5::scamp5_in))
        .method("scamp5_load_in", select_overload<void(const std::shared_ptr<AREG>&, int8_t, std::shared_ptr<AREG>&)>(&SCAMP5::scamp5_load_in))
        .method("scamp5_load_in", select_overload<void(const std::shared_ptr<AREG>&, int8_t)>(&SCAMP5::scamp5_load_in))
        .method("scamp5_load_in", select_overload<void(int8_t)>(&SCAMP5::scamp5_load_in))
        .method("scamp5_load_dac", select_overload<void(const std::shared_ptr<AREG>&, uint16_t, std::shared_ptr<AREG>&)>(&SCAMP5::scamp5_load_dac))
        .method("scamp5_load_dac", select_overload<void(const std::shared_ptr<AREG>&, uint16_t)>(&SCAMP5::scamp5_load_dac))
        .method("scamp5_load_dac", select_overload<void(uint16_t)>(&SCAMP5::scamp5_load_dac))
        .method("scamp5_shift", select_overload<void(const std::shared_ptr<AREG>&, int, int)>(&SCAMP5::scamp5_shift))
        .method("scamp5_diffuse", select_overload<void(const std::shared_ptr<AREG>&, int, bool, bool, std::shared_ptr<AREG>&)>(&SCAMP5::scamp5_diffuse))
        .method("scamp5_diffuse", select_overload<void(const std::shared_ptr<AREG>&, int)>(&SCAMP5::scamp5_diffuse))
        .method("scamp5_diffuse", select_overload<void(const std::shared_ptr<AREG>&, int, bool)>(&SCAMP5::scamp5_diffuse))
        .method("scamp5_diffuse", select_overload<void(const std::shared_ptr<AREG>&, int, bool, bool)>(&SCAMP5::scamp5_diffuse))
        .method("scamp5_read_areg", &SCAMP5::scamp5_read_areg)
        .method("scamp5_global_sum_16", &SCAMP5::scamp5_global_sum_16)(default_arguments(nullptr))
        .method("scamp5_global_sum_64", &SCAMP5::scamp5_global_sum_64)(default_arguments(nullptr))
        .method("scamp5_global_sum_fast", &SCAMP5::scamp5_global_sum_fast)
        .method("scamp5_global_sum_sparse", &SCAMP5::scamp5_global_sum_sparse)(default_arguments((uint8_t)2, (uint8_t)2, (uint8_t)254, (uint8_t)254))
        .method("scamp5_shift", select_overload<void(const std::shared_ptr<DREG>&, int, int, int)>(&SCAMP5::scamp5_shift))(default_arguments(0))
        .method("scamp5_global_or", &SCAMP5::scamp5_global_or)(default_arguments((uint8_t)0, (uint8_t)0, (uint8_t)255, (uint8_t)255))
        .method("scamp5_global_count", &SCAMP5::scamp5_global_count)(default_arguments(0))
        .method("scamp5_flood", &SCAMP5::scamp5_flood)(default_arguments(5, false))
        .method("scamp5_load_point", &SCAMP5::scamp5_load_point)
        .method("scamp5_load_rect", &SCAMP5::scamp5_load_rect)
        .method("scamp5_load_pattern", &SCAMP5::scamp5_load_pattern)
        .method("scamp5_select_point", &SCAMP5::scamp5_select_point)
        .method("scamp5_select_rect", &SCAMP5::scamp5_select_rect)
        .method("scamp5_select_pattern", &SCAMP5::scamp5_select_pattern)
        .method("scamp5_select_col", &SCAMP5::scamp5_select_col)
        .method("scamp5_select_row", &SCAMP5::scamp5_select_row)
        .method("scamp5_select_colx", &SCAMP5::scamp5_select_colx)
        .method("scamp5_select_rowx", &SCAMP5::scamp5_select_rowx)
        .method("scamp5_draw_begin", &SCAMP5::scamp5_draw_begin)
        .method("scamp5_draw_end", &SCAMP5::scamp5_draw_end)
        .method("scamp5_draw_pixel", &SCAMP5::scamp5_draw_pixel)
        .method("scamp5_draw_point", &SCAMP5::scamp5_draw_point)
        .method("scamp5_draw_rect", &SCAMP5::scamp5_draw_rect)
        .method("scamp5_draw_line", &SCAMP5::scamp5_draw_line)(default_arguments(false))
        .method("scamp5_draw_circle", &SCAMP5::scamp5_draw_circle)(default_arguments(false))
        .method("scamp5_draw_negate", &SCAMP5::scamp5_draw_negate)
        .method("scamp5_scan_areg", &SCAMP5::scamp5_scan_areg)
        .method("scamp5_scan_areg_8x8", &SCAMP5::scamp5_scan_areg_8x8)
        .method("scamp5_scan_areg_mean_8x8", &SCAMP5::scamp5_scan_areg_mean_8x8)
        .method("scamp5_scan_dreg", &SCAMP5::scamp5_scan_dreg)(default_arguments((uint8_t)0, (uint8_t)255))
        .method("scamp5_scan_events", select_overload<void(const std::shared_ptr<DREG>&, uint8_t*, uint16_t, uint8_t, uint8_t)>(&SCAMP5::scamp5_scan_events))(default_arguments((uint16_t)1000, (uint8_t)0, (uint8_t)0))
        .method("scamp5_scan_events", select_overload<void(const std::shared_ptr<DREG>&, uint8_t*, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)>(&SCAMP5::scamp5_scan_events))
        .method("scamp5_scan_boundingbox", &SCAMP5::scamp5_scan_boundingbox)
        .method("print_stats", &SCAMP5::print_stats);
}
