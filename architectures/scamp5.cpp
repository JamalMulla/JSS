//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"

SCAMP5::SCAMP5() {
    // Initially all PEs are active
    this->FLAG.value().setTo(1);
}


void SCAMP5::nop() { }

void SCAMP5::rpix() {
    //reset PIX
    this->pe.photodiode.reset();
}

void SCAMP5::get_image(AREG &y) {
    //y := half-range image, and reset PIX
    this->pe.photodiode.read(PIX);
    this->bus(NEWS, PIX);
    this->rpix();
    this->rpix();
    this->nop();
    this->bus(y, NEWS, PIX);
}

void SCAMP5::get_image(AREG &y, AREG &h) {
    //y := full-range image, h := negative half-range image, and reset PIX
    this->pe.photodiode.read(PIX);
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
    this->bus(NEWS, PIX);
    this->bus(y, NEWS);
}

void SCAMP5::getpix(AREG &y, AREG &pix_res) {
    //y := half-range image, supplying the reset level of PIX
    this->bus(NEWS,PIX);
    this->bus(y,NEWS, pix_res);
}

void SCAMP5::getpix(AREG &y, AREG &h, AREG &pix_res) {
    //y := full-range, h := half-range image, supplying the reset level of PIX
    this->bus(h,PIX);
    this->bus(NEWS, PIX);
    this->bus(y, h, NEWS, pix_res);
}

void SCAMP5::bus(AREG &a) {
    // a = 0 + error
    this->pe.analogue_bus.bus(a, FLAG);
}

void SCAMP5::bus(AREG &a, const AREG &a0) {
    // a = -a0 + error
    this->pe.analogue_bus.bus(a, a0, FLAG);
}

void SCAMP5::bus(AREG &a, const AREG &a0, const AREG &a1) {
    this->pe.analogue_bus.bus(a, a0, a1,FLAG);
}

void SCAMP5::bus(AREG &a, const AREG &a0, const AREG &a1, const AREG &a2) {
    this->pe.analogue_bus.bus(a, a0, a1, a2,FLAG);
}

void SCAMP5::bus(AREG &a, const AREG &a0, const AREG &a1, const AREG &a2, const AREG &a3) {
    this->pe.analogue_bus.bus(a, a0, a1, a2,a3, FLAG);
}

void SCAMP5::bus2(AREG &a, AREG &b) {
    this->pe.analogue_bus.bus2(a, b, FLAG);
}

void SCAMP5::bus2(AREG &a, AREG &b, const AREG &a0) {
    this->pe.analogue_bus.bus2(a, b, a0,FLAG);
}

void SCAMP5::bus2(AREG &a, AREG &b, const AREG &a0, const AREG &a1) {
    this->pe.analogue_bus.bus2(a, b, a0, a1,FLAG);
}

void SCAMP5::bus3(AREG &a, AREG &b, AREG &c, const AREG &a0) {
    this->pe.analogue_bus.bus3(a, b, c, a0, FLAG);
}

void SCAMP5::where(const AREG &a) {
    //FLAG := a > 0.
    this->pe.analogue_bus.conditional_positive_set(a, FLAG);
}

void SCAMP5::where(const AREG &a0, const AREG &a1) {
    //FLAG := (a0 + a1) > 0.
    this->pe.analogue_bus.conditional_positive_set(a0, a1, FLAG);
}

void SCAMP5::where(const AREG &a0, const AREG &a1, const AREG &a2) {
    //FLAG := (a0 + a1 + a2) > 0.
    this->pe.analogue_bus.conditional_positive_set(a0, a1, a2, FLAG);
}

void SCAMP5::all() {
    //FLAG := 1.
    this->FLAG.set();
}

void SCAMP5::mov(AREG &y, const AREG &x0) {
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

void SCAMP5::add(AREG &y, const AREG &x0, const AREG &x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void SCAMP5::add(AREG &y, const AREG &x0, const AREG &x1, const AREG &x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void SCAMP5::sub(AREG &y, const AREG &x0, const AREG &x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void SCAMP5::neg(AREG &y, const AREG &x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void SCAMP5::abs(AREG &y, const AREG &x0) {
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

void SCAMP5::div(AREG &y0, AREG &y1, AREG &y2, const AREG &x0) {
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

void SCAMP5::divq(AREG &y0, const AREG &x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}


// TODO Check neighbour access implementations

void SCAMP5::movx(AREG &y, const AREG &x0, news_t dir) {
    // y = x0_dir
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(x0, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(x0, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(x0, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(x0, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS);
}

void SCAMP5::mov2x(AREG &y, const AREG &x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir (note: this only works when FLAG is "all")
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(x0, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(x0, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(x0, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(x0, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.push_north(NEWS, y, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_east(NEWS, y, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_west(NEWS, y, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_south(NEWS, y, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
}

void SCAMP5::addx(AREG &y, const AREG &x0, const AREG &x1, news_t dir) {
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

void SCAMP5::add2x(AREG &y, const AREG &x0, const AREG &x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    AREG intermediate(y.value().rows, y.value().cols);
    this->bus(intermediate, x0, x1);
    this->neg(intermediate, intermediate);
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(intermediate, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(intermediate, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(intermediate, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(intermediate, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.push_north(NEWS, y, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_east(NEWS, y, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_west(NEWS, y, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_south(NEWS, y, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
}

void SCAMP5::subx(AREG &y, const AREG &x0, news_t dir, const AREG &x1) {
    // y := x0_dir - x1
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(x0, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(x0, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(x0, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(x0, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->neg(NEWS, NEWS);
    this->bus(y, NEWS, x1);
}

void SCAMP5::sub2x(AREG &y, const AREG &x0, news_t dir, news_t dir2, const AREG &x1) {
    // y := x0_dir_dir2 - x1
    switch (dir) {
        case north: this->pe.analogue_bus.push_south(x0, NEWS, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_west(x0, NEWS, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_east(x0, NEWS, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_north(x0, NEWS, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    switch (dir2) {
        case north: this->pe.analogue_bus.push_north(NEWS, y, 1, FLAG); break;
        case east: this->pe.analogue_bus.push_east(NEWS, y, 1, FLAG); break;
        case west: this->pe.analogue_bus.push_west(NEWS, y, 1, FLAG); break;
        case south: this->pe.analogue_bus.push_south(NEWS, y, 1, FLAG); break;
        case alldir: std::cerr << "Unhandled direction" << std::endl; break;
    }
    this->bus(y, NEWS, x1);
}

void SCAMP5::blurset() {

}

void SCAMP5::blur(AREG &a, AREG &a0) {

}

void SCAMP5::blurh(AREG &a, AREG &a0) {

}

void SCAMP5::blurv(AREG &a, AREG &a0) {

}

void SCAMP5::gauss(AREG &y, AREG &x, const int iterations = 3) {
    // blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly

}

void SCAMP5::gaussh(AREG &y, AREG &x, const int iterations) {
    // horizontally blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly
}

void SCAMP5::gaussv(AREG &y, AREG &x, const int iterations) {
    // vertically blur x into y with constant number of iterations (x and y can be same AREG),
    // require R1 and R2 to be set properly
}

void SCAMP5::newsblur(AREG &y, AREG &x, const int iterations) {
    // blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
}

void SCAMP5::newsblurh(AREG &y, AREG &x, const int iterations) {
    // horizontally blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
}

void SCAMP5::newsblurv(AREG &y, AREG &x, const int iterations) {
    // vertically blur x into y with constant number of iterations using neighbour mixing (x and y can be same AREG)
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1) {
    // d := d0 OR d1
    DigitalBus::OR(d, d0, FLAG);
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2) {
    // d := d0 OR d1 OR d2
    DigitalBus::OR(d, d0, d1, d2, FLAG);
}

void SCAMP5::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d := d0 OR d1 OR d2 OR d3
    DigitalBus::OR(d, d0, d1, d2, d3, FLAG);
}

void SCAMP5::NOT(DREG &d, DREG &d0) {
    // d := NOT d0
    DigitalBus::NOT(d, d0, FLAG);
}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1) {
    // d := NOT(d0 OR d1)
    DigitalBus::NOR(d, d0, d1, FLAG);
}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::NOR(d, d0, d1, d2,FLAG);
}

void SCAMP5::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::NOR(d, d0, d1, d2,d3, FLAG);
}

void SCAMP5::NOT(DREG &Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
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
    this->FLAG.value().setTo(d.value());
}

void SCAMP5::WHERE(DREG &d0, DREG &d1) {
    // FLAG := d0 OR d1.
    DREG intermediate(d0.value().rows, d0.value().cols);
    this->OR(intermediate, d0, d1);
    this->FLAG.value().setTo(intermediate.value());
}

void SCAMP5::WHERE(DREG &d0, DREG &d1, DREG &d2) {
    // FLAG := d0 OR d1 OR d2.
    DREG intermediate(d0.value().rows, d0.value().cols);
    this->OR(intermediate, d0, d1, d2);
    this->FLAG.value().setTo(intermediate.value());
}

void SCAMP5::ALL() {
    // FLAG := 1, same as all.
    this->FLAG.set();
}

void SCAMP5::SET(DREG &d0) {
    // d0 := 1
    d0.set(FLAG);
}

void SCAMP5::SET(DREG &d0, DREG &d1) {
    // d0, d1 := 1
    d0.set(FLAG);
    d1.set(FLAG);
}

void SCAMP5::SET(DREG &d0, DREG &d1, DREG &d2) {
    // 	d0, d1, d2 := 1
    d0.set(FLAG);
    d1.set(FLAG);
    d2.set(FLAG);
}

void SCAMP5::SET(DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // d0, d1, d2, d3 := 1
    d0.set(FLAG);
    d1.set(FLAG);
    d2.set(FLAG);
    d3.set(FLAG);
}

void SCAMP5::CLR(DREG &d0) {
    // d0 := 0
    d0.clear(FLAG);
}

void SCAMP5::CLR(DREG &d0, DREG &d1) {
    // d0, d1 := 0
    d0.clear(FLAG);
    d1.clear(FLAG);
}

void SCAMP5::CLR(DREG &d0, DREG &d1, DREG &d2) {
    // d0, d1, d2 := 0
    d0.clear(FLAG);
    d1.clear(FLAG);
    d2.clear(FLAG);
}

void SCAMP5::CLR(DREG &d0, DREG &d1, DREG &d2, DREG &d3) {
    // 	d0, d1, d2, d3 := 0
    d0.clear(FLAG);
    d1.clear(FLAG);
    d2.clear(FLAG);
    d3.clear(FLAG);
}

void SCAMP5::MOV(DREG &d, DREG &d0) {
    // d := d0
    DigitalBus::MOV(d, d0, FLAG);
}

void SCAMP5::MUX(DREG &Rl, DREG &Rx, DREG &Ry, DREG &Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    DigitalBus::MUX(Rl, Rx, Ry, Rz, FLAG);
}

void SCAMP5::CLR_IF(DREG &Rl, DREG &Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalBus::CLR_IF(Rl, Rx, FLAG);
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







