//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"

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
    this->pe.analogue_bus.bus(a, FLAG);
}

void SCAMP5::bus(AREG &a, const AREG &a0) {
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

}

void SCAMP5::bus2(AREG &a, AREG &b, const AREG &a0, const AREG &a1) {

}

void SCAMP5::bus3(AREG &a, AREG &b, AREG &c, const AREG &a0) {

}

void SCAMP5::where(const AREG &a) {

}

void SCAMP5::where(const AREG &a0, const AREG &a1) {

}

void SCAMP5::where(const AREG &a0, const AREG &a1, const AREG &a2) {

}

void SCAMP5::all() {

}

void SCAMP5::mov(AREG &y, const AREG &x0) {

}


