//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"

#include <simulator/memory/sram6t_cell.h>
#include <simulator/util/utility.h>
#include <rttr/registration>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <utility>


SCAMP5::SCAMP5(int rows, int cols, Origin origin)
    : rows_(rows), cols_(cols), origin_(origin), config_(1e7) {

    pe = std::make_shared<ProcessingElement>(
        ProcessingElement::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_analogue_registers(ANALOGUE_REGISTERS)
            .with_digital_registers(DIGITAL_REGISTERS)
            .with_input_source(Source::LIVE)
            .with_config(config_)
            .build());
    array = std::make_shared<Array>(rows, cols, config_, *pe);
    this->init();
}

void SCAMP5::init() {
    PIX = &array->pe.analogue_registers[0]("PIX");
    IN = &array->pe.analogue_registers[1]("IN");
    NEWS = &array->pe.analogue_registers[2]("NEWS");
    A = &array->pe.analogue_registers[3]("A");
    B = &array->pe.analogue_registers[4]("B");
    C = &array->pe.analogue_registers[5]("C");
    D = &array->pe.analogue_registers[6]("D");
    E = &array->pe.analogue_registers[7]("E");
    F = &array->pe.analogue_registers[8]("F");

    FLAG = &array->pe.digital_registers[0]("FLAG");
    RF = &array->pe.digital_registers[1]("RF");
    RS = &array->pe.digital_registers[2]("RS");
    RW = &array->pe.digital_registers[3]("RW");
    RN = &array->pe.digital_registers[4]("RN");
    RE = &array->pe.digital_registers[5]("RE");
    S0 = &array->pe.digital_registers[6]("S0");
    S1 = &array->pe.digital_registers[7]("S1");
    S2 = &array->pe.digital_registers[8]("S2");
    S3 = &array->pe.digital_registers[9]("S3");
    S4 = &array->pe.digital_registers[10]("S4");
    S5 = &array->pe.digital_registers[11]("S5");
    S6 = &array->pe.digital_registers[12]("S6");
    RP = &array->pe.digital_registers[13]("RP");
    SELECT = &array->pe.digital_registers[14]("SELECT");
    RECT = &array->pe.digital_registers[15]("RECT");

    // Initially all PEs are active
    FLAG->write(1);

    FLAG->change_memory_type(MemoryType::SRAM6T);

    intermediate_a = std::make_shared<AREG>(this->rows_, this->cols_);
    intermediate_a2 = std::make_shared<AREG>(this->rows_, this->cols_);
    intermediate_d = std::make_shared<DREG>(this->rows_, this->cols_);
}

void SCAMP5::set_bitorder(Bitorder bitorder) {
    this->bitorder_ = std::move(bitorder);
    this->superpixel_size_ = this->bitorder_.at(0).size();
    std::vector<std::vector<int>> bank = this->bitorder_.at(0);
    // bits_in_bank is number of non zeros in bank
    this->bits_in_bank_ = 0;

    for (int row = 0; row < superpixel_size_; ++row) {
        for (int col = 0; col < superpixel_size_; ++col) {
            if (bank[row][col] > 0) {
                bits_in_bank_++;
            }
        }
    }
}

void SCAMP5::nop() { this->array->update_cycles(1); }

void SCAMP5::rpix() {
    // reset *PIX
    this->pe->photodiode.reset();
    this->array->update_cycles(1);
}

void SCAMP5::get_image(AREG *y) {
    // y := half-range image, and reset *PIX
    this->pe->photodiode.read(*PIX);
    this->array->update_cycles(10000);
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void SCAMP5::get_image(AREG *y, AREG *h) {
    // y := full-range image, h := negative half-range image, and reset *PIX
    this->pe->photodiode.read(*PIX);
//    this->array->update_static(420000); //todo
    this->array->update_cycles(100); //2000
//    this->array->update_static(this->pe->photodiode.get_cycle_count());
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

void SCAMP5::respix(AREG *y) {
    // reset *PIX, keep its reset level in y
    this->rpix();
    this->rpix();
    this->nop();
    this->pe->photodiode.read(*PIX);
    this->array->update_cycles(30000); //todo
//    this->array->update_static(this->pe->photodiode.get_cycle_count());
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void SCAMP5::getpix(AREG *y, AREG *pix_res) {
    // y := half-range image, supplying the reset level of *PIX
    this->pe->photodiode.read(*PIX);
    this->array->update_cycles(30000); //todo
//    this->array->update_static(this->pe->photodiode.get_cycle_count());
    this->bus(NEWS, PIX);
    this->bus(y, NEWS, pix_res);
}

void SCAMP5::getpix(AREG *y, AREG *h, AREG *pix_res) {
    // y := full-range, h := half-range image, supplying the reset level of *PIX
    this->pe->photodiode.read(*PIX);
    this->array->update_cycles(30000); //todo
//    this->array->update_static(this->pe->photodiode.get_cycle_count());
    this->bus(h, PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

void SCAMP5::bus(AREG *a) {
    // a = 0 + error
    this->pe->analogue_bus.bus(*a, *FLAG);
    this->array->update_cycles(1);
}

void SCAMP5::bus(AREG *a, AREG *a0) {
    // a = -a0 + error
    this->pe->analogue_bus.bus(*a, *a0, *FLAG);
    this->array->update_cycles(1);
}

void SCAMP5::bus(AREG *a, AREG *a0, AREG *a1) {
    // a = -(a0 + a1) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *FLAG);
    this->array->update_cycles(4); // 2 reads, 1 add, 1 write
}

void SCAMP5::bus(AREG *a, AREG *a0, AREG *a1, AREG *a2) {
    // a = -(a0 + a1 + a2) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *FLAG);
    this->array->update_cycles(5);  // 3 reads, 1 add, 1 write
}

void SCAMP5::bus(AREG *a, AREG *a0, AREG *a1, AREG *a2, AREG *a3) {
    // a = -(a0 + a1 + a2 + a3) + error
    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *a3, *FLAG);
    this->array->update_cycles(6);  // 4 reads, 1 add, 1 write
}

void SCAMP5::bus2(AREG *a, AREG *b) {
    // a,b = 0 + error
    this->pe->analogue_bus.bus2(*a, *b, *FLAG);
    this->array->update_cycles(2);  // 2 writes
}

void SCAMP5::bus2(AREG *a, AREG *b, AREG *a0) {
    // a,b = -0.5*a0 + error + noise
    this->pe->analogue_bus.bus2(*a, *b, *a0, *FLAG);
    this->array->update_cycles(3);  // 1 read, 2 writes
}

void SCAMP5::bus2(AREG *a, AREG *b, AREG *a0, AREG *a1) {
    // a,b = -0.5*(a0 + a1) + error + noise
    this->pe->analogue_bus.bus2(*a, *b, *a0, *a1, *FLAG);
    this->array->update_cycles(5);  // 2 reads, 1 add, 2 writes
}

void SCAMP5::bus3(AREG *a, AREG *b, AREG *c, AREG *a0) {
    // a,b,c = -0.33*a0 + error + noise
    this->pe->analogue_bus.bus3(*a, *b, *c, *a0, *FLAG);
    this->array->update_cycles(2);  // 1 read, 3 writes
}

void SCAMP5::where(AREG *a) {
    // FLAG := a > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a);
    this->array->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::where(AREG *a0, AREG *a1) {
    // FLAG := (a0 + a1) > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a0, *a1);
    this->array->update_cycles(4);  // 2 reads, 1 add, 1 write
}

void SCAMP5::where(AREG *a0, AREG *a1, AREG *a2) {
    // FLAG := (a0 + a1 + a2) > 0.
    this->pe->analogue_bus.conditional_positive_set(*FLAG, *a0, *a1, *a2);
    this->array->update_cycles(5);  // 3 reads, 1 add, 1 write
}

void SCAMP5::all() {
    // FLAG := 1.
    this->FLAG->set();
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::mov(AREG *y, AREG *x0) {
    // y = x0
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

void SCAMP5::res(AREG *a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void SCAMP5::res(AREG *a, AREG *b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void SCAMP5::add(AREG *y, AREG *x0, AREG *x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void SCAMP5::add(AREG *y, AREG *x0, AREG *x1, AREG *x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void SCAMP5::sub(AREG *y, AREG *x0, AREG *x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void SCAMP5::neg(AREG *y, AREG *x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void SCAMP5::abs(AREG *y, AREG *x0) {
    // y = |x0|
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void SCAMP5::div(AREG *y0, AREG *y1, AREG *y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::div(AREG *y0, AREG *y1, AREG *y2, AREG *x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void SCAMP5::diva(AREG *y0, AREG *y1, AREG *y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void SCAMP5::divq(AREG *y0, AREG *x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

void SCAMP5::movx(AREG *y, AREG *x0, news_t dir) {
    // y = x0_dir
    this->bus(intermediate_a.get(), x0);
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
    this->array->update_cycles(1);  // movement?
}

void SCAMP5::mov2x(AREG *y, AREG *x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir2 (note: this only works when FLAG is "all")
    this->bus(intermediate_a.get(), x0);
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
    this->bus(y, intermediate_a2.get());
    this->array->update_cycles(2);  // movement
}

void SCAMP5::addx(AREG *y, AREG *x0, AREG *x1, news_t dir) {
    // y := x0_dir + x1_dir
    this->bus(intermediate_a.get(), x0, x1);
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
    this->array->update_cycles(1);  //movement
}

void SCAMP5::add2x(AREG *y, AREG *x0, AREG *x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    this->bus(intermediate_a.get(), x0, x1);
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
    this->bus(y, intermediate_a2.get());
    this->array->update_cycles(2);  // movement
}

void SCAMP5::subx(AREG *y, AREG *x0, news_t dir, AREG *x1) {
    // y := x0_dir - x1
    this->bus(intermediate_a.get(), x0);
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
    this->array->update_cycles(1);  // movement
}

void SCAMP5::sub2x(AREG *y, AREG *x0, news_t dir, news_t dir2, AREG *x1) {
    // y := x0_dir_dir2 - x1
    this->bus(intermediate_a.get(), x0);
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
    this->bus(y, intermediate_a2.get(), x1);
    this->array->update_cycles(2);  // movement
}

void SCAMP5::blurset() {
    // need to be used once ahead of a blur instruction
    // TODO check
    R1->set();
    R2->set();
    this->array->update_cycles(2);  // 2 writes
}

void SCAMP5::blur(AREG *a, AREG *a0) {
    // blur a0 into a
    // full blur across array.
    cv::GaussianBlur(a0->read(), a->read(), cv::Size(3, 3), 0);
    cv::bitwise_not(a->read(), a->read());
    this->array->update_cycles(sqrt(rows_ * cols_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::blurh(AREG *a, AREG *a0) {
    // analog asynchronized blur, horizontal only
    // row vector for x direction which is horizontal?
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat horizontal_kernel = cv::Mat(1, 6, CV_32F, gaussian_1d);

    cv::filter2D(a0->read(), a->read(), -1, horizontal_kernel);
    cv::bitwise_not(a->read(), a->read());
    this->array->update_cycles(sqrt(rows_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::blurv(AREG *a, AREG *a0) {
    // analog asynchronized blur, vertical only
    // blur a0 into a
    float gaussian_1d[10] = {0.006, 0.061, 0.242, 0.383, 0.242, 0.061, 0.006};
    cv::Mat vertical_kernel = cv::Mat(6, 1, CV_32F, gaussian_1d);

    cv::filter2D(a0->read(), a->read(), -1, vertical_kernel);
    cv::bitwise_not(a->read(), a->read());
    this->array->update_cycles(sqrt(cols_)); // TODO get reasonable numbers for the cycle count
}

void SCAMP5::gauss(AREG *y, AREG *x, const int iterations) {
    // blur x into y with constant number of iterations (x and y can be same
    // AREG), require R1 and R2 to be set properly
    blur(NEWS, x);
    blur(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blur(NEWS, y);
        blur(y, NEWS);
    }
}

void SCAMP5::gaussh(AREG *y, AREG *x, const int iterations) {
    // horizontally blur x into y with constant number of iterations (x and y
    // can be same AREG), require R1 and R2 to be set properly
    blurh(NEWS, x);
    blurh(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blurh(NEWS, y);
        blurh(y, NEWS);
    }
}

void SCAMP5::gaussv(AREG *y, AREG *x, const int iterations) {
    // vertically blur x into y with constant number of iterations (x and y can
    // be same AREG), require R1 and R2 to be set properly
    blurv(NEWS, x);
    blurv(y, NEWS);
    for(int i = 1; i < iterations; i++) {
        blurv(NEWS, y);
        blurv(y, NEWS);
    }
}

void SCAMP5::newsblur(AREG *y, AREG *x, const int iterations) {
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
        this->array->update_cycles(sqrt(rows_ * cols_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::newsblurh(AREG *y, AREG *x, const int iterations) {
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
        this->array->update_cycles(sqrt(rows_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::newsblurv(AREG *y, AREG *x, const int iterations) {
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
        this->array->update_cycles(sqrt(cols_)); // TODO get reasonable numbers for the cycle count
    }
}

void SCAMP5::OR(DREG *d, DREG *d0, DREG *d1) {
    // d := d0 OR d1
    this->pe->local_read_bus.OR(*d, *d0, *d1);
    this->array->update_cycles(4); // 2 reads, 1 op, 1 write
}

void SCAMP5::OR(DREG *d, DREG *d0, DREG *d1, DREG *d2) {
    // d := d0 OR d1 OR d2
    this->pe->local_read_bus.OR(*d, *d0, *d1, *d2);
    this->array->update_cycles(5);  // 3 reads, 1 op, 1 write
}

void SCAMP5::OR(DREG *d, DREG *d0, DREG *d1, DREG *d2, DREG *d3) {
    // d := d0 OR d1 OR d2 OR d3
    this->pe->local_read_bus.OR(*d, *d0, *d1, *d2, *d3);
    this->array->update_cycles(6);  // 4 reads, 1 op, 1 write
}

void SCAMP5::NOT(DREG *d, DREG *d0) {
    // d := NOT d0
    this->pe->local_read_bus.NOT(*d, *d0);
    this->array->update_cycles(3); // 1 read, 1 op, 1 write
}

void SCAMP5::NOR(DREG *d, DREG *d0, DREG *d1) {
    // d := NOR(d0 OR d1)
    this->pe->local_read_bus.NOR(*d, *d0, *d1);
    this->array->update_cycles(5);  // 2 reads, 2 op, 1 write
}

void SCAMP5::NOR(DREG *d, DREG *d0, DREG *d1, DREG *d2) {
    // d := NOR(d0 OR d1 OR d2)
    this->pe->local_read_bus.NOR(*d, *d0, *d1, *d2);
    this->array->update_cycles(6);  // 3 reads, 2 op, 1 write
}

void SCAMP5::NOR(DREG *d, DREG *d0, DREG *d1, DREG *d2, DREG *d3) {
    // d := NOTRd0 OR d1 OR d2 OR d3)
    this->pe->local_read_bus.NOR(*d, *d0, *d1, *d2, *d3);
    this->array->update_cycles(7); // 4 reads, 2 op, 1 write
}

void SCAMP5::NOT(DREG *Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
    this->array->update_cycles(4); // 2 reads, 1 op, 1 write
}

void SCAMP5::OR(DREG *Rl, DREG *Rx) {
    // Rl := Rl OR Rx
    this->OR(Rl, Rl, Rx);
}

void SCAMP5::NOR(DREG *Rl, DREG *Rx) {
    // Rl := Rl NOR Rx
    this->NOR(Rl, Rl, Rx);
}

void SCAMP5::AND(DREG *Ra, DREG *Rx, DREG *Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->NOR(Ra, RF, RP);
}

void SCAMP5::NAND(DREG *Ra, DREG *Rx, DREG *Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->OR(Ra, RF, RP);
}

void SCAMP5::ANDX(DREG *Ra, DREG *Rb, DREG *Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, RF, Rb);
}

void SCAMP5::NANDX(DREG *Ra, DREG *Rb, DREG *Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, RF, Rb);
}

void SCAMP5::IMP(DREG *Rl, DREG *Rx, DREG *Ry) {
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

void SCAMP5::NIMP(DREG *Rl, DREG *Rx, DREG *Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(RF, Ry);
    this->NOR(RS, Rx, RF);
}

void SCAMP5::XOR(DREG *Rl, DREG *Rx, DREG *Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(RF, Ry);
    this->NOR(Rl, Rx, RF);
    this->NOT(RF, Rx);
    this->NOR(Rx, Ry, RF);
    this->OR(Rl, Rx);
}

void SCAMP5::WHERE(DREG *d) {
    // FLAG := d.
    this->FLAG->write(d->read());
    this->array->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::WHERE(DREG *d0, DREG *d1) {
    // FLAG := d0 OR d1.
    this->OR(intermediate_d.get(), d0, d1);
    this->FLAG->write(intermediate_d->read());
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::WHERE(DREG *d0, DREG *d1, DREG *d2) {
    // FLAG := d0 OR d1 OR d2.
    this->OR(intermediate_d.get(), d0, d1, d2);
    this->FLAG->write(intermediate_d->read());
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::ALL() {
    // FLAG := 1, same as all.
    this->FLAG->set();
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::SET(DREG *d0) {
    // d0 := 1
    d0->set();
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::SET(DREG *d0, DREG *d1) {
    // d0, d1 := 1
    d0->set();
    d1->set();
    this->array->update_cycles(2);  // 2 writes
}

void SCAMP5::SET(DREG *d0, DREG *d1, DREG *d2) {
    // 	d0, d1, d2 := 1
    d0->set();
    d1->set();
    d2->set();
    this->array->update_cycles(3);  // 3 writes
}

void SCAMP5::SET(DREG *d0, DREG *d1, DREG *d2, DREG *d3) {
    // d0, d1, d2, d3 := 1
    d0->set();
    d1->set();
    d2->set();
    d3->set();
    this->array->update_cycles(4);  // 4 writes
}

void SCAMP5::CLR(DREG *d0) {
    // d0 := 0
    d0->clear();
    this->array->update_cycles(1);  // 1 write
}

void SCAMP5::CLR(DREG *d0, DREG *d1) {
    // d0, d1 := 0
    d0->clear();
    d1->clear();
    this->array->update_cycles(2);  // 2 writes
}

void SCAMP5::CLR(DREG *d0, DREG *d1, DREG *d2) {
    // d0, d1, d2 := 0
    d0->clear();
    d1->clear();
    d2->clear();
    this->array->update_cycles(3);  // 3 writes
}

void SCAMP5::CLR(DREG *d0, DREG *d1, DREG *d2, DREG *d3) {
    // 	d0, d1, d2, d3 := 0
    d0->clear();
    d1->clear();
    d2->clear();
    d3->clear();
    this->array->update_cycles(4);  // 4 writes
}

void SCAMP5::MOV(DREG *d, DREG *d0) {
    // d := d0
    this->pe->local_read_bus.MOV(*d, *d0);
    this->array->update_cycles(2);  // 1 read, 1 write
}

void SCAMP5::MUX(DREG *Rl, DREG *Rx, DREG *Ry, DREG *Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    this->pe->local_read_bus.MUX(*Rl, *Rx, *Ry, *Rz);
    this->array->update_cycles(4);  // 3 reads, 1 write, some op?
}

void SCAMP5::CLR_IF(DREG *Rl, DREG *Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    this->pe->local_read_bus.CLR_IF(*Rl, *Rx);
    this->array->update_cycles(2);  // 1 read, up to 1 write, some op for if?
}

void SCAMP5::REFRESH(DREG *Rl) {
    // 	refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds)
    // without any operations
}

void SCAMP5::DNEWS0(DREG *d, DREG *d0) {
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

    AND(&east, &east, RE);
    AND(&north, &north, RN);
    AND(&west, &west, RW);
    AND(&south, &south, RS);

    OR(d, &east, &north, &south, &west);
}

void SCAMP5::DNEWS1(DREG *d, DREG *d0) {
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

    AND(&east, &east, RE);
    AND(&north, &north, RN);
    AND(&west, &west, RW);
    AND(&south, &south, RS);

    OR(d, &east, &north, &south, &west);
}

void SCAMP5::DNEWS(DREG *Ra, DREG *Rx, int dir, bool boundary) {
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

void SCAMP5::scamp5_get_image(AREG *yf, AREG *yh, int gain) {
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
    this->pe->photodiode.read(*PIX);
    bus(NEWS, PIX);
    rpix();
    this->pe->photodiode.read(*PIX);
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
    this->pe->photodiode.read(*PIX);
    bus(NEWS, yh, yf, PIX);
    bus(yf, NEWS);
}

void SCAMP5::scamp5_in(AREG *areg, int16_t value, AREG *temp) {
    // load an analog value to the AREG with error&noise correction
    // TODO noise
    // TODO Pointer instead of reference. Need member as default somehow
    if(temp == nullptr) {
        temp = NEWS;
    }
    IN->write(value);
    this->array->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_load_in(AREG *areg, int8_t value, AREG *temp) {
    // load a analog value to the AREG plane without error&noise correction
    // TODO noise
    if(temp == nullptr) {
        temp = NEWS;
    }
    IN->write(value);
    this->array->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_load_in(int8_t value) {
    // load a analog value to IN without error&noise correction
    // TODO noise
    IN->write(value);
    this->array->update_cycles(1);
}

void SCAMP5::scamp5_load_dac(AREG *areg, uint16_t value, AREG *temp) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	(optional) temporary kernel register to be used in the function
    if(temp == nullptr) {
        temp = NEWS;
    }
    IN->write(value);
    this->array->update_cycles(1);
    bus(temp, IN);
    bus(areg, temp);
}

void SCAMP5::scamp5_load_dac(uint16_t value) {
    // load an analog value to IN using a raw DAC value
    // TODO What is with the range of values here. Why can some registers hold a
    // much larger range?
    IN->write(value);
    this->array->update_cycles(1);
}

void SCAMP5::scamp5_shift(AREG *areg, int h, int v) {
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
    //        AREG *direction = *(h > 0 ? EAST : WEST);
    //        AREG *end = (h % 2 == 0 ? direction : *NEWS);
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

void SCAMP5::scamp5_diffuse(AREG *target, int iterations, bool vertical,
                            bool horizontal, AREG *t0) {
    // diffuse an AREG image
    // TODO is this the same as Gaussian blur?
    void (SCAMP5::*blur_func)(AREG *, AREG *);
    if(horizontal && vertical) {
        blur_func = &SCAMP5::blur;
    } else if(horizontal) {
        blur_func = &SCAMP5::blurh;
    } else {
        blur_func = &SCAMP5::blurv;
    }

    if(t0 == nullptr) {
        t0 = NEWS;
    }

    (*this.*blur_func)(t0, target);
    (*this.*blur_func)(target, t0);
    for(int i = 1; i < iterations; i++) {
        (*this.*blur_func)(t0, target);
        (*this.*blur_func)(target, t0);
    }
}

uint8_t SCAMP5::scamp5_read_areg(AREG *areg, uint8_t r, uint8_t c) {
    // read a single pixel
    // TODO check that the value is properly mapped to uint8_t from CV_16U
    return areg->read().at<uint8_t>(r, c);
}

uint32_t SCAMP5::scamp5_global_sum_16(AREG *areg, uint8_t *result16v) {
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
    this->array->update_cycles(140); // 14usec at 10MHz. TODO make flexible
    return sum;
}

uint32_t SCAMP5::scamp5_global_sum_64(AREG *areg, uint8_t *result64v) {
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
    this->array->update_cycles(280); // 28usec at 10MHz. TODO improve
    return sum;
}

uint8_t SCAMP5::scamp5_global_sum_fast(AREG *areg) {
    // get approximate sum level of the whole AREG plane
    // TODO should be approximate sum not exact. Also need to abstract away cv call
    this->array->update_cycles(70); // TODO improve
    return cv::sum(areg->read())[0];
}

uint8_t SCAMP5::scamp5_global_sum_sparse(AREG *areg, uint8_t r, uint8_t c,
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
    this->array->update_cycles(70); // TODO improve
    return sum;
}

void SCAMP5::scamp5_shift(DREG *dreg, int h, int v, const int boundary) {
    // shift a DREG image
}

int SCAMP5::scamp5_global_or(DREG *dreg, uint8_t r, uint8_t c, uint8_t rx,
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
    this->array->update_cycles(70); // TODO improve
    return val;
}

int SCAMP5::scamp5_global_count(DREG *dreg, AREG *temp, int options) {
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

void SCAMP5::scamp5_flood(DREG *dreg_target, DREG *dreg_mask, int boundary,
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

void SCAMP5::scamp5_load_point(DREG *dr, uint8_t r, uint8_t c) {
    // set a single pixel on a DREG image to 1, the rest to 0
    dr->read().setTo(0);
    dr->read().at<uint8_t>(r, c) = 1;
}

void SCAMP5::scamp5_load_rect(DREG *dr, uint8_t r0, uint8_t c0, uint8_t r1,
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

void SCAMP5::scamp5_load_pattern(DREG *dr, uint8_t r, uint8_t c, uint8_t rx,
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

void SCAMP5::scamp5_draw_begin(DREG *dr) {
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

void SCAMP5::scamp5_scan_areg(AREG *areg, uint8_t *buffer, uint8_t r0,
                              uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs,
                              uint8_t cs) {
    // scan a customized grid of pixels in an AREG image
    // Note, the result image is stored in column-major format, starting from
    // top right. i.e. "buffer[3]" is the pixel the on 1st column right, 4th row
    // down. This applies to all "scamp5_scan_areg_*" series functions.
    this->pe->analogue_bus.scan(buffer, *areg, r0, c0, r1, c1, rs, cs, this->origin_);
}

void SCAMP5::scamp5_scan_areg_8x8(AREG *areg, uint8_t *result8x8) {
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

void SCAMP5::scamp5_scan_areg_mean_8x8(AREG *areg, uint8_t *result8x8) {
    // divide the AREG image into 8x8 square blocks, and get the average of each
    // block result8x8 - pointer to a buffer to store the results
    this->pe->analogue_bus.blocked_average(result8x8, *areg, 8, this->origin_);
}

void SCAMP5::scamp5_scan_dreg(DREG *dreg, uint8_t *mem, uint8_t r0,
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

void SCAMP5::scamp5_scan_events(DREG *dreg, uint8_t *mem, uint16_t max_num,
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

void SCAMP5::scamp5_scan_events(DREG *dreg, uint8_t *buffer, uint16_t max_num,
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

void SCAMP5::scamp5_scan_boundingbox(DREG *dr, uint8_t *vec4v) {
    // scan the bounding box coordinates of '1's in a DREG image
    // The coordinates are two points: the top-right and the bottom-left corners
    // of the axis-aligned bounding box (AABB) of '1's in the DREG. vec4v -
    // pointer to a buffer of 4 byte
}

// Simulator specific

void SCAMP5::print_stats() {
    // TODO move
#ifdef TRACK_STATISTICS
    this->array->update_static(); //move
    this->array->print_stats();
//    json j;
//    j["Total number of cycles"] = counter->get_cycles();
//    j["Equivalent in seconds"] = counter->to_seconds(config_.clock_rate);
//
//    // this->array.print_stats(counter);
//    this->array->write_stats(*counter, j);
//    std::cout << std::setw(2) << j << std::endl;
//    std::ofstream file_out;
//    std::cout << std::filesystem::current_path().string() << std::endl;
//    file_out.open(std::filesystem::current_path().string() + "/output.json");
//    file_out << std::setw(2) << j;
//    file_out.close();
#endif
}

// EXTENSIONS

// Superpixel methods

void SCAMP5::superpixel_positions_from_bitorder(position_map &locations) {
    // Locations holds a map from <bank, index> -> x,y coords
    if (bitorder_.empty()) {
        std::cerr << "[Error] Bitorder has not been defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    int banks = bitorder_.size();
    int height = bitorder_[0].size();
    int width = bitorder_[0][0].size();

    for(int b = 0; b < banks; b++) {
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                int index = bitorder_[b][h][w];
                if(index < 1) continue;  // Bitorder indices start 1
                locations[{b, index}] = cv::Point(w, h);
            }
        }
    }
}

void SCAMP5::superpixel_shift_patterns_from_bitorder(int bank, DREG* RNORTH, DREG* RSOUTH, DREG* REAST, DREG* RWEST, bool shift_left) {
    size_t rows = bitorder_[0].size();
    size_t cols = bitorder_[0][0].size();
    DigitalRegister R_NORTH(rows, cols);
    DigitalRegister R_SOUTH(rows, cols);
    DigitalRegister R_EAST(rows, cols);
    DigitalRegister R_WEST(rows, cols);

    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < cols; col++) {
            int north;
            int west;
            int current = bitorder_[bank][row][col];
            if(current < 1) continue;  // Indices start at 1
            if(row == 0) {
                // No north so set north to current
                north = current;
            } else {
                north = bitorder_[bank][row - 1][col];
            }

            if(col == 0) {
                // No west so set west to current
                west = current;
            } else {
                west = bitorder_[bank][row][col - 1];
            }

            if(current == north + 1) {
                // bigger than north
                (shift_left ? R_SOUTH : R_NORTH).read().at<uint8_t>(row - (shift_left ? 0 : 1), col) = 1;
            } else if(current == north - 1) {
                // smaller than north
                (shift_left ? R_NORTH : R_SOUTH).read().at<uint8_t>(row - (shift_left ? 1 : 0), col) = 1;
            }

            if(current == west + 1) {
                // bigger than west
                (shift_left ? R_EAST : R_WEST).read().at<uint8_t>(row, col - (shift_left ? 0 : 1)) = 1;
            } else if(current == west - 1) {
                // smaller than west
                (shift_left ? R_WEST : R_EAST).read().at<uint8_t>(row, col - (shift_left ? 1 : 0)) = 1;
            }
        }
    }

    switch(origin_) {
        case BOTTOM_LEFT: {
            RSOUTH->write(R_NORTH);
            RNORTH->write(R_SOUTH);
            REAST->write(R_WEST);
            RWEST->write(R_EAST);
            break;
        }
        case BOTTOM_RIGHT: {
            RSOUTH->write(R_NORTH);
            RNORTH->write(R_SOUTH);
            REAST->write(R_EAST);
            RWEST->write(R_WEST);
            break;
        }
        case TOP_LEFT: {
            // opencv default
            RSOUTH->write(R_SOUTH);
            RNORTH->write(R_NORTH);
            REAST->write(R_WEST);
            RWEST->write(R_EAST);
            break;
        }
        case TOP_RIGHT: {
            RSOUTH->write(R_SOUTH);
            RNORTH->write(R_NORTH);
            REAST->write(R_EAST);
            RWEST->write(R_WEST);
            break;
        }
    }
}

void SCAMP5::superpixel_shift_block(DREG* dst, DREG* src, DREG* RNORTH, DREG* RSOUTH, DREG* REAST, DREG* RWEST) {
    int rows = src->read().rows;
    int cols = src->read().cols;
    DigitalRegister east = DigitalRegister(rows, cols);
    DigitalRegister north = DigitalRegister(rows, cols);
    DigitalRegister west = DigitalRegister(rows, cols);
    DigitalRegister south = DigitalRegister(rows, cols);

    this->pe->local_read_bus.get_east(east, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_north(north, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_west(west, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_south(south, *src, 1, 0, origin_);

    AND(&east, &east, REAST);
    AND(&north, &north, RNORTH);
    AND(&west, &west, RWEST);
    AND(&south, &south, RSOUTH);

    OR(dst, &east, &north, &south, &west);
}

void SCAMP5::superpixel_adc(DREG *dst, int bank, AREG *src) {
    // Converts an analogue image to a digital superpixel format
    // Values will always be put in bank 0
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);

    cv::Mat &d = dst->read();
    cv::Mat &sr = src->read();
    parallel_for_(cv::Range(0, sr.rows * sr.cols), [&](const cv::Range &range) {
      for(int r = range.start; r < range.end; r++) {
          int row = r / sr.cols;
          int col = r % sr.cols;

          if(row % superpixel_size_ != 0) continue;  // Step size is not 1
          if(col % superpixel_size_ != 0) continue;

          int sum = cv::sum(sr(cv::Rect(col, row, superpixel_size_, superpixel_size_)))[0];
          sum /= (superpixel_size_ * superpixel_size_);  // <- this truncates values
          int8_t s = sum;  // TODO Need to have another look at this. Is this correct?
          for(int i = 0; i < bits_in_bank_; i++) {
              int bit = (s >> i) & 1;  // LSB to MSB
              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
              d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
          }
      }
    });
}

void SCAMP5::superpixel_dac(AREG *dst, int bank, DREG *src) {
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);
    // Converts digital superpixel format image to an analogue image

    cv::Mat &d = dst->read();
    cv::Mat &s = src->read();
    parallel_for_(cv::Range(0, s.rows * s.cols), [&](const cv::Range &range) {
      for(int r = range.start; r < range.end; r++) {
          int row = r / s.cols;
          int col = r % s.cols;

          if(row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
          if(col % superpixel_size_ != 0) continue;

          // Read value from superpixel
          int value = 0;
          for(int i = 0; i < bits_in_bank_; i++) {
              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
              int bit = s.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
              value |= bit << i;  // LSB to MSB
          }
          d(cv::Rect(col, row, superpixel_size_, superpixel_size_)) = value;
      }
    });
}

void SCAMP5::superpixel_in(DREG *dst, int bank, int value) {
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);
    auto bits = std::make_unique<int[]>(bits_in_bank_);

    // Loads a constant value in each superpixel
    for(int i = 0; i < bits_in_bank_; i++) {
        int bit = (value >> i) & 1;  // LSB to MSB
        bits[i] = bit;
    }

    cv::Mat &d = dst->read();
    for(int col = 0; col < d.cols; col += superpixel_size_) {
        for(int row = 0; row < d.rows; row += superpixel_size_) {
            for(int i = 0; i < bits_in_bank_; i++) {
                cv::Point relative_pos = locations.at({bank, i + 1});
                d.at<uint8_t>(relative_pos.y + row,relative_pos.x + col) = bits[i];
            }
        }
    }
}

void SCAMP5::superpixel_shift(DREG *dst, int bank, DREG *src, int shift_left) {
    int rows = src->read().rows;
    int cols = src->read().cols;
    DigitalRegister RNORTH = DigitalRegister(superpixel_size_, superpixel_size_);
    DigitalRegister RSOUTH = DigitalRegister(superpixel_size_, superpixel_size_);
    DigitalRegister REAST = DigitalRegister(superpixel_size_, superpixel_size_);
    DigitalRegister RWEST = DigitalRegister(superpixel_size_, superpixel_size_);
    superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST,
                                            shift_left);

    // TODO non-square superpixels?
    int num_of_repeats_y = rows / superpixel_size_;
    int num_of_repeats_x = cols / superpixel_size_;
    DigitalRegister R_NORTH = cv::repeat(RNORTH.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_SOUTH = cv::repeat(RSOUTH.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_EAST = cv::repeat(REAST.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_WEST = cv::repeat(RWEST.read(), num_of_repeats_y, num_of_repeats_x);
    superpixel_shift_block(dst, src, &R_NORTH, &R_SOUTH, &R_EAST, &R_WEST);

    if(bitorder_.size() > 1) {
        // only need to preserve other banks if we have more than 1 bank
        DigitalRegister block_mask = DigitalRegister(superpixel_size_, superpixel_size_);

        cv::Mat &bm = block_mask.read();
        for(size_t b = 0; b < bitorder_.size(); b++) {
            for(size_t row = 0; row < superpixel_size_; row++) {
                for(size_t col = 0; col < superpixel_size_; col++) {
                    int current = bitorder_[b][row][col];
                    if(current < 1) continue;  // Indices start at 1
                    if(b != bank) {
                        // Set mask bit to 1 everywhere other than the bank
                        bm.at<uint8_t>(row, col) = 1;
                    }
                }
            }
        }

        DigitalRegister mask = cv::repeat(bm, num_of_repeats_y, num_of_repeats_x);
        DigitalRegister and_ = DigitalRegister(rows, cols);

        AND(&and_, &mask, src);
        OR(dst, &and_, dst);
    }
}

void SCAMP5::superpixel_shift_right(DREG *dst, int bank, DREG *src) {
    // Logical shift right
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, false);
}
void SCAMP5::superpixel_shift_left(DREG *dst, int bank, DREG *src) {
    // Logical shift left
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, true);
}

//void SCAMP5::superpixel_add(DREG *dst, int bank, DREG* src1, DREG* src2) {
//    // Clobbers R11
//    DigitalRegister A = src1->read().clone();
//    DigitalRegister B = src2->read().clone();
//    DigitalRegister and_ = DigitalRegister(src1->read().rows, src1->read().cols);
//    DigitalRegister xor_ = DigitalRegister(src1->read().rows, src1->read().cols);
//
//    AND(&and_, &A, &B);
//
//    while(cv::sum(and_.read())[0] != 0) {
//        MOV(R11, &A); // Needed because XOR clobbers A
//        XOR(&xor_, &A, &B);
//        MOV(&A, R11);
//        AND(&and_, &A, &B);
//        superpixel_shift_left(&and_, bank, &and_);
//        A.write(xor_);
//        B.write(and_);
//        AND(&and_, &A, &B);
//    }
//    MOV(R11, &A);
//    XOR(dst, &A, &B);
//    MOV(&A, R11);
//}

void SCAMP5::superpixel_add(DREG *dst, int bank, DREG* src1, DREG* src2) {
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);

    cv::Mat &d = dst->read();
    cv::Mat &s1 = src1->read();
    cv::Mat &s2 = src2->read();
    parallel_for_(cv::Range(0, rows_ * cols_), [&](const cv::Range &range) {
      for(int r = range.start; r < range.end; r++) {
          int row = r / cols_;
          int col = r % cols_;

          if(row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
          if(col % superpixel_size_ != 0) continue;

          // Read value from each superpixel
          int value1 = 0;
          int value2 = 0;
          for(int i = 0; i < bits_in_bank_; i++) {
              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
              int bit = s1.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
              value1 |= bit << i;  // LSB to MSB
              int bit2 = s2.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
              value2 |= bit2 << i;  // LSB to MSB
          }

          int sum = this->array->cla.add(value1, value2);  // Need to have another look at this. Is this correct?
          for(int i = 0; i < bits_in_bank_; i++) {
              int bit = (sum >> i) & 1;  // LSB to MSB
              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
              d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
          }
      }
    });
#ifdef TRACK_STATISTICS
    this->array->cla.inc_add();
#endif
    this->array->update_cycles(1);
}


void SCAMP5::superpixel_sub(DREG *dst, int bank, DREG* src1, DREG* src2) {
    // Clobbers R11
    DigitalRegister A = src1->read().clone();
    DigitalRegister B = src2->read().clone();
    DigitalRegister NOT_A = src1->read().clone();
    DigitalRegister and_ =
        DigitalRegister(src1->read().rows, src1->read().cols);
    DigitalRegister xor_ =
        DigitalRegister(src1->read().rows, src1->read().cols);

    NOT(&NOT_A, &A);
    AND(&and_, &NOT_A, &B);

    while(cv::sum(and_.read())[0] != 0) {
        MOV(R11, &A); // Needed because XOR clobbers A
        XOR(&xor_, &A, &B);
        MOV(&A, R11);
        NOT(&NOT_A, &A);
        AND(&and_, &NOT_A, &B);
        superpixel_shift_left(&and_, bank, &and_);
        A.write(xor_);
        B.write(and_);
        NOT(&NOT_A, &A);
        AND(&and_, &NOT_A, &B);
    }
    MOV(R11, &A);
    XOR(dst, &A, &B);
    MOV(&A, R11);
}

//void SCAMP5::superpixel_sub(DREG *dst, int bank, DREG* src1, DREG* src2) {
//    position_map locations;
//    this->superpixel_positions_from_bitorder(locations);
//
//    cv::Mat &d = dst->read();
//    cv::Mat &s1 = src1->read();
//    cv::Mat &s2 = src2->read();
//    parallel_for_(cv::Range(0, rows_ * cols_), [&](const cv::Range &range) {
//      for(int r = range.start; r < range.end; r++) {
//          int row = r / cols_;
//          int col = r % cols_;
//
//          if(row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
//          if(col % superpixel_size_ != 0) continue;
//
//          // Read value from each superpixel
//          int value1 = 0;
//          int value2 = 0;
//          for(int i = 0; i < bits_in_bank_; i++) {
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              int bit = s1.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value1 |= bit << i;  // LSB to MSB
//              int bit2 = s2.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value2 |= bit2 << i;  // LSB to MSB
//          }
//
//          int8_t sum = this->array->cla.add(-value1, value2);  // Need to have another look at this. Is this correct?
//          for(int i = 0; i < bits_in_bank_; i++) {
//              int bit = (sum >> i) & 1;  // LSB to MSB
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
//          }
//      }
//    });
//#ifdef TRACK_STATISTICS
//    this->array->cla.inc_add();
//#endif
//    this->array->update_cycles(1);
//}

void SCAMP5::superpixel_movx(DREG *dst, DREG* src, news_t dir) {
    for(int i = 0; i < superpixel_size_; ++i) {
        DNEWS(dst, src, dir, 0);
        src = dst;
    }
}

// Histogramming

void SCAMP5::histogram(AREG* src) {
    // Need a block size. There is a tradeoff between the number of arrays and the time it takes as you can only write 1 value at a time
    this->array->dram.reset();
    int blocksize = 8;
    int block = 0;
    cv::Mat& s = src->read();

    for (int row = 0; row < rows_; row += blocksize) {
        for (int col = 0; col < cols_; col += blocksize) {

            for (int r = row; r < row + blocksize; r++) {
                for (int c = col; c < col + blocksize; c++) {
                    int m_row = s.at<int16_t>(r, c) + 128; // intensity is used as index
                    // need to shift by 128 to get a 0-255 range
                    int m_col = 0;

                    int value = this->array->dram.read(block, m_row, m_col);
                    this->array->dram.write(block, m_row, m_col, value+1);
                }
            }
            block++;
        }
    }

    // Combine all the dram cells into one by just adding up each of the dram arrays for each value
    std::unordered_map<int, int> histogram;

    for (int i = 0; i < block; i++) {
        // init to 0
        histogram[i] = 0;
    }

    int cells = (rows_*cols_)/blocksize/blocksize; // number of dram arrays across array
    for (int i = 0; i < block; i++) {
        int combined = 0;
        for (int b = 0; b < cells; b++) { //todo parameterise properly
            int i1 = this->array->dram.read(b, i, 0);
            combined += i1;
        }
        
        histogram[i] = combined;
    }

    std::cout << "x = [";
    for (int i = 0; i < block; i++) {
        std::cout << i << ",";
    }
    std::cout << "]" << std::endl;

    std::cout << "y = [";
    for (int i = 0; i < block; i++) {
        std::cout << histogram[i] << ",";
    }
    std::cout << "]" << std::endl;

}

void SCAMP5::hog(AREG *src) {
    // Clobbers A, B
    this->array->dram.reset(); //todo time and energy

    scamp5_load_in(F, 127);
    add(src, src, F);

    // calculate gradients gx, gy using sobel
    neg(B, src);
    subx(C, src, west, B);
    subx(E, src, east, B);
    subx(B, B, south, src);
    movx(A, B, north);
    addx(C, C, E, south);
    addx(B, B, A, west);
    sub2x(A, B, east, east, B);
    sub2x(B, C, north, north, C);
    // A = gy
    // B = gx

    cv::Mat A_float;
    cv::Mat B_float;
    A->read().convertTo(A_float, CV_32F, 1/255.0);
    B->read().convertTo(B_float, CV_32F, 1/255.0);

    // calculate gradient magnitude and direction (in degrees)
    cv::Mat m; //magnitudes
    cv::Mat a; //angles
    cv::cartToPolar(A_float, B_float, m, a, true);
    this->array->update_cycles(350); // todo better number here for arctan
    // todo also need to add power here that would come from carrying out this computation

    // calculate Histogram of Gradients in 8×8 cells
    int blocksize = 8;
    int bucket_size = 20;
    int num_buckets = 9;

    int block = 0;
    for (int row = 0; row < rows_; row += blocksize) {
        for (int col = 0; col < cols_; col += blocksize) {

            for (int r = row; r < row + blocksize; r++) {
                for (int c = col; c < col + blocksize; c++) {
                    float magnitude = m.at<float>(r, c);
                    int angle = (int)a.at<float>(r, c) % 180; // unsigned only
                    int m_col = 0;

                    int b1 = floor((double) angle/bucket_size);
                    int b2 = (b1 + 1) % num_buckets;

                    double v2 = ceil(((double) (angle - (b1 * bucket_size))/bucket_size) * magnitude);
                    double v1 = ceil(magnitude - v2);

                    int value = this->array->dram.read(block, b1, m_col);
                    this->array->dram.write(block, b1, m_col, value+v1);

                    int value2 = this->array->dram.read(block, b2, m_col);
                    this->array->dram.write(block, b2, m_col, value2+v2);
                }
            }
            block++;
        }
    }

    // output histograms for each block
    for (int b = 0; b < block; b++) {
        std::cout << "B:" << b;
        for (int i = 0; i < 10; i++) { //todo parameterise properly
            int i1 = this->array->dram.read(b, i, 0);
            std::cout << ", Bin: " <<  i*20 << ", Count: " << i1 << " |";
        }
        std::cout << std::endl;
    }


    // 16×16 Block Normalization
    // Calculate the HOG feature vector
}

// Builder

rttr::variant SCAMP5::builder::bitorder_converter(json& j) {
    try {
        auto b = j.get<Bitorder>();
        return b;
    } catch (json::type_error&) {
        std::cerr << "[Warning] Could not parse bitorder. Must be a 3D array" << std::endl;
    } catch (json::parse_error &) {
        std::cerr << "[Warning] Could not parse bitorder. Must be a 3D array" << std::endl;
    }

    return rttr::variant();
}

SCAMP5::builder &SCAMP5::builder::with_rows(int rows) {
    this->rows_ = rows;
    return *this;
}

SCAMP5::builder &SCAMP5::builder::with_cols(int cols) {
    this->cols_ = cols;
    return *this;
}

SCAMP5::builder &SCAMP5::builder::with_origin(Origin origin) {
    this->origin_ = origin;
    return *this;
}

SCAMP5::builder &SCAMP5::builder::with_bitorder(Bitorder bitorder) {
    this->bitorder_ = std::move(bitorder);
    return *this;
}

SCAMP5 SCAMP5::builder::build() {
    SCAMP5 scamp5 = SCAMP5(this->rows_, this->cols_, this->origin_);
    if (!this->bitorder_.empty()) {
        scamp5.set_bitorder(this->bitorder_);
    }
    return scamp5;
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::enumeration<news_t>("news_t") (
        value("north", news_t::north),
        value("east", news_t::east),
        value("south", news_t::south),
        value("west", news_t::west)
    );

    registration::class_<SCAMP5::builder>("SCAMP5_builder")
        .constructor<>()
        .method("bitorder_converter", &SCAMP5::builder::bitorder_converter)
        .method("with_rows", &SCAMP5::builder::with_rows)
        .method("with_cols", &SCAMP5::builder::with_cols)
        .method("with_origin", &SCAMP5::builder::with_origin)
        .method("with_bitorder", &SCAMP5::builder::with_bitorder)
        .method("build", &SCAMP5::builder::build);

    registration::class_<SCAMP5>("SCAMP5")
        .property("A", &SCAMP5::A)
        .property("B", &SCAMP5::B)
        .property("C", &SCAMP5::C)
        .property("D", &SCAMP5::D)
        .property("E", &SCAMP5::E)
        .property("F", &SCAMP5::F)
        .property("FLAG", &SCAMP5::FLAG)
        .property("RF", &SCAMP5::R0)
        .property("RS", &SCAMP5::R1)
        .property("RW", &SCAMP5::R2)
        .property("RN", &SCAMP5::R3)
        .property("RE", &SCAMP5::R4)
        .property("S0", &SCAMP5::R5)
        .property("S1", &SCAMP5::R6)
        .property("S2", &SCAMP5::R7)
        .property("S3", &SCAMP5::R8)
        .property("S4", &SCAMP5::R9)
        .property("S5", &SCAMP5::R10)
        .property("S6", &SCAMP5::R11)
        .property("RP", &SCAMP5::R12)
        .property("R0", &SCAMP5::R0)
        .property("R1", &SCAMP5::R1)
        .property("R2", &SCAMP5::R2)
        .property("R3", &SCAMP5::R3)
        .property("R4", &SCAMP5::R4)
        .property("R5", &SCAMP5::R5)
        .property("R6", &SCAMP5::R6)
        .property("R7", &SCAMP5::R7)
        .property("R8", &SCAMP5::R8)
        .property("R9", &SCAMP5::R9)
        .property("R10", &SCAMP5::R10)
        .property("R11", &SCAMP5::R11)
        .property("R12", &SCAMP5::R12)
        .method("nop", &SCAMP5::nop)
        .method("rpix", &SCAMP5::rpix)
        .method("get_image", select_overload<void(AREG*)> (&SCAMP5::get_image))
        .method("get_image", select_overload<void(AREG*, AREG*)>(&SCAMP5::get_image))
        .method("respix", select_overload<void()>(&SCAMP5::respix))
        .method("respix", select_overload<void(AREG*)>(&SCAMP5::respix))
        .method("getpix", select_overload<void(AREG*, AREG*)>(&SCAMP5::getpix))
        .method("getpix", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::getpix))
        .method("bus", select_overload<void(AREG*)>(&SCAMP5::bus))
        .method("bus", select_overload<void(AREG*, AREG*)>(&SCAMP5::bus))
        .method("bus", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::bus))
        .method("bus", select_overload<void(AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::bus))
        .method("bus", select_overload<void(AREG*, AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::bus))
        .method("bus2", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::bus2))
        .method("bus2", select_overload<void(AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::bus2))
        .method("bus3", select_overload<void(AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::bus3))
        .method("where", select_overload<void(AREG*)>(&SCAMP5::where))
        .method("where", select_overload<void(AREG*, AREG*)>(&SCAMP5::where))
        .method("where", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::where))
        .method("all", &SCAMP5::all)
        .method("mov", &SCAMP5::mov)
        .method("res", select_overload<void(AREG*)>(&SCAMP5::res))
        .method("res", select_overload<void(AREG*, AREG*)>(&SCAMP5::res))
        .method("add", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::add))
        .method("add", select_overload<void(AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::add))
        .method("sub", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::sub))
        .method("neg", &SCAMP5::neg)
        .method("abs", &SCAMP5::abs)
        .method("div", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::div))
        .method("div", select_overload<void(AREG*, AREG*, AREG*, AREG*)>(&SCAMP5::div))
        .method("diva", select_overload<void(AREG*, AREG*, AREG*)>(&SCAMP5::diva))
        .method("divq", select_overload<void(AREG*, AREG*)>(&SCAMP5::divq))
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
        .method("OR", select_overload<void(DREG*, DREG*, DREG*)>(&SCAMP5::OR))
        .method("OR", select_overload<void(DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::OR))
        .method("OR", select_overload<void(DREG*, DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::OR))
        .method("NOT", select_overload<void(DREG*, DREG*)>(&SCAMP5::NOT))
        .method("NOR", select_overload<void(DREG*, DREG*, DREG*)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(DREG*, DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::NOR))
        .method("NOR", select_overload<void(DREG*)>(&SCAMP5::NOT))
        .method("OR", select_overload<void(DREG*, DREG*)>(&SCAMP5::OR))
        .method("NOR", select_overload<void(DREG*, DREG*)>(&SCAMP5::NOR))
        .method("AND", &SCAMP5::AND)
        .method("NAND", &SCAMP5::NAND)
        .method("ANDX", &SCAMP5::ANDX)
        .method("NANDX", &SCAMP5::NANDX)
        .method("IMP", &SCAMP5::IMP)
        .method("NIMP", &SCAMP5::NIMP)
        .method("XOR", &SCAMP5::XOR)
        .method("WHERE", select_overload<void(DREG*)>(&SCAMP5::WHERE))
        .method("WHERE", select_overload<void(DREG*, DREG*)>(&SCAMP5::WHERE))
        .method("WHERE", select_overload<void(DREG*, DREG*, DREG*)>(&SCAMP5::WHERE))
        .method("ALL", &SCAMP5::ALL)
        .method("SET", select_overload<void(DREG*)>(&SCAMP5::SET))
        .method("SET", select_overload<void(DREG*, DREG*)>(&SCAMP5::SET))
        .method("SET", select_overload<void(DREG*, DREG*, DREG*)>(&SCAMP5::SET))
        .method("SET", select_overload<void(DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::SET))
        .method("CLR", select_overload<void(DREG*)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(DREG*, DREG*)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(DREG*, DREG*, DREG*)>(&SCAMP5::CLR))
        .method("CLR", select_overload<void(DREG*, DREG*, DREG*, DREG*)>(&SCAMP5::CLR))
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
        .method("scamp5_in", &SCAMP5::scamp5_in)(default_arguments(nullptr))
        .method("scamp5_load_in", select_overload<void(AREG*, int8_t, AREG*)>(&SCAMP5::scamp5_load_in))(default_arguments(nullptr))
        .method("scamp5_load_in", select_overload<void(int8_t)>(&SCAMP5::scamp5_load_in))
        .method("scamp5_load_dac", select_overload<void(AREG*, uint16_t, AREG*)>(&SCAMP5::scamp5_load_dac))(default_arguments(nullptr))
        .method("scamp5_load_dac", select_overload<void(uint16_t)>(&SCAMP5::scamp5_load_dac))
        .method("scamp5_shift", select_overload<void(AREG*, int, int)>(&SCAMP5::scamp5_shift))
        .method("scamp5_diffuse", select_overload<void(AREG*, int, bool, bool, AREG*)>(&SCAMP5::scamp5_diffuse))(default_arguments(true, true, nullptr))
        .method("scamp5_read_areg", &SCAMP5::scamp5_read_areg)
        .method("scamp5_global_sum_16", &SCAMP5::scamp5_global_sum_16)(default_arguments(nullptr))
        .method("scamp5_global_sum_64", &SCAMP5::scamp5_global_sum_64)(default_arguments(nullptr))
        .method("scamp5_global_sum_fast", &SCAMP5::scamp5_global_sum_fast)
        .method("scamp5_global_sum_sparse", &SCAMP5::scamp5_global_sum_sparse)(default_arguments((uint8_t)2, (uint8_t)2, (uint8_t)254, (uint8_t)254))
        .method("scamp5_shift", select_overload<void(DREG*, int, int, int)>(&SCAMP5::scamp5_shift))(default_arguments(0))
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
        .method("scamp5_scan_events", select_overload<void(DREG*, uint8_t*, uint16_t, uint8_t, uint8_t)>(&SCAMP5::scamp5_scan_events))(default_arguments((uint16_t)1000, (uint8_t)0, (uint8_t)0))
        .method("scamp5_scan_events", select_overload<void(DREG*, uint8_t*, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)>(&SCAMP5::scamp5_scan_events))
        .method("scamp5_scan_boundingbox", &SCAMP5::scamp5_scan_boundingbox)
        .method("superpixel_positions_from_bitorder", &SCAMP5::superpixel_positions_from_bitorder)
        .method("superpixel_shift_patterns_from_bitorder", &SCAMP5::superpixel_shift_patterns_from_bitorder)
        .method("superpixel_adc", &SCAMP5::superpixel_adc)
        .method("superpixel_dac", &SCAMP5::superpixel_dac)
        .method("superpixel_in", &SCAMP5::superpixel_in)
        .method("superpixel_shift", &SCAMP5::superpixel_shift)
        .method("superpixel_shift_right", &SCAMP5::superpixel_shift_right)
        .method("superpixel_shift_left", &SCAMP5::superpixel_shift_left)
        .method("superpixel_add", &SCAMP5::superpixel_add)
        .method("superpixel_sub", &SCAMP5::superpixel_sub)
        .method("superpixel_movx", &SCAMP5::superpixel_movx)
        .method("histogram", &SCAMP5::histogram)
        .method("hog", &SCAMP5::hog);
}
