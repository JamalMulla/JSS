//
// Created by jm1417 on 28/01/2021.
//

#include <opencv2/core.hpp>
#include "simulator/buses/digital_bus.h"

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1) {
    // d := d0 OR d1
    cv::bitwise_or(d0.value(), d1.value(), d.value());
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
}

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2) {
    // d := d0 OR d1 OR d2
    cv::bitwise_or(d0.value(), d1.value(), d.value());
    cv::bitwise_or(d.value(), d2.value(), d.value());
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
}

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &d3) {
    // d := d0 OR d1 OR d2 OR d3
    cv::bitwise_or(d0.value(), d1.value(), d.value());
    cv::bitwise_or(d.value(), d2.value(), d.value());
    cv::bitwise_or(d.value(), d3.value(), d.value());
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
    d3.inc_write();
}

void DigitalBus::NOT(DigitalRegister &d, DigitalRegister &d0) {
    // d := NOT d0
    cv::bitwise_not(d0.value(), d.value());
    d.inc_write();
    d0.inc_write();
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1) {
    // d := NOT(d0 OR d1)
    DigitalBus::OR(d, d0, d1);
    DigitalBus::NOT(d);
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::OR(d, d0, d1, d2);
    DigitalBus::NOT(d);
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::OR(d, d0, d1, d2, d3);
    DigitalBus::NOT(d);
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
    d3.inc_read();
}

void DigitalBus::NOT(DigitalRegister &Rl) {
    // Rl := NOT Rl
    cv::bitwise_not(Rl.value(), Rl.value());
    Rl.inc_write();
    Rl.inc_read();
}

void DigitalBus::OR(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := Rl OR Rx
    DigitalBus::OR(Rl, Rl, Rx);
    Rl.inc_write();
    Rl.inc_read();
    Rx.inc_read();
}

void DigitalBus::NOR(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := Rl NOR Rx
    DigitalBus::NOR(Rl, Rl, Rx);
    Rl.inc_write();
    Rl.inc_read();
    Rx.inc_read();
}

void DigitalBus::AND(DigitalRegister &Ra, DigitalRegister &Rx, DigitalRegister &Ry) {
    // Ra := Rx AND Ry
    cv::bitwise_and(Rx.value(), Ry.value(), Ra.value());
    Ra.inc_write();
    Rx.inc_read();
    Ry.inc_read();
}

void DigitalBus::NAND(DigitalRegister &Ra, DigitalRegister &Rx, DigitalRegister &Ry) {
    // Ra = NOT(Rx AND Ry)
    DigitalBus::AND(Ra, Rx, Ry);
    DigitalBus::NOT(Ra);
    Ra.inc_write();
    Rx.inc_read();
    Ry.inc_read();
}

void DigitalBus::IMP(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOT(intermediate, Ry);
    DigitalBus::OR(Rl, Rx, intermediate);
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
}

void DigitalBus::NIMP(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry) {
    // Rl := Rx NIMP Ry
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOT(intermediate);
    DigitalBus::NOR(Rl, Rx, intermediate);
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
}

void DigitalBus::XOR(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry) {
    // Rl := Rx XOR Ry
    cv::bitwise_xor(Rx.value(), Ry.value(), Rl.value());
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
}

void DigitalBus::MOV(DigitalRegister &d, DigitalRegister &d0) {
    // d := d0
    cv::copyTo(d0.value(), d.value(), cv::noArray());
    d.inc_write();
    d0.inc_read();
}

void DigitalBus::MUX(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    // R1 = (Ry.~Rx) + (Rz.Rx)
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalRegister intermediate2(Rl.value().rows, Rl.value().cols);
    cv::bitwise_not(Rx.value(), intermediate.value());
    DigitalBus::AND(intermediate2, Ry, intermediate);
    DigitalBus::AND(intermediate, Rz, Rx);
    DigitalBus::OR(Rl, intermediate, intermediate2);
    //TODO fix reads
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
    Rz.inc_read();
}

void DigitalBus::CLR_IF(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOT(intermediate, Rl);
    DigitalBus::NOR(Rl, intermediate, Rx);
    //TODO fix reads
    Rl.inc_write();
    Rx.inc_read();
}

// Masked

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &FLAG) {
    // d := d0 OR d1
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
}

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &FLAG) {
    // d := d0 OR d1 OR d2
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d2.value(), d.value(), FLAG.value());
}

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &d3, DigitalRegister &FLAG) {
    // d := d0 OR d1 OR d2 OR d3
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d2.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d3.value(), d.value(), FLAG.value());
}

void DigitalBus::NOT_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &FLAG) {
    // d := NOT d0
    cv::bitwise_not(d0.value(), d.value(), FLAG.value());
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1)
    DigitalBus::OR_MASKED(d, d0, d1, FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::OR_MASKED(d, d0, d1, d2,FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &d1, DigitalRegister &d2, DigitalRegister &d3, DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::OR_MASKED(d, d0, d1, d2,d3, FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOT_MASKED(DigitalRegister &Rl, DigitalRegister &FLAG) {
    // Rl := NOT Rl
    cv::bitwise_not(Rl.value(), Rl.value(), FLAG.value());
}

void DigitalBus::OR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &FLAG) {
    // Rl := Rl OR Rx
    DigitalBus::OR_MASKED(Rl, Rl, Rx, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &FLAG) {
    // Rl := Rl NOR Rx
    DigitalBus::NOR_MASKED(Rl, Rl, Rx, FLAG);
}

void DigitalBus::AND_MASKED(DigitalRegister &Ra, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Ra := Rx AND Ry
    cv::bitwise_and(Rx.value(), Ry.value(), Ra.value(), FLAG.value());
}

void DigitalBus::NAND_MASKED(DigitalRegister &Ra, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Ra = NOT(Rx AND Ry)
    DigitalBus::AND_MASKED(Ra, Rx, Ry, FLAG);
    DigitalBus::NOT_MASKED(Ra, FLAG);
}

void DigitalBus::IMP_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOT_MASKED(intermediate, Ry, FLAG);
    DigitalBus::OR_MASKED(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::NIMP_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx NIMP Ry
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOR_MASKED(intermediate, Ry, FLAG);
    DigitalBus::NOR_MASKED(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::XOR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx XOR Ry
    cv::bitwise_xor(Rx.value(), Ry.value(), Rl.value(), FLAG.value());
}

void DigitalBus::MOV_MASKED(DigitalRegister &d, DigitalRegister &d0, DigitalRegister &FLAG) {
    // d := d0
    cv::copyTo(d0.value(), d.value(), FLAG.value());
}

void DigitalBus::MUX_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &Ry, DigitalRegister &Rz, DigitalRegister &FLAG) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    // R1 = (Ry.~Rx) + (Rz.Rx)
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalRegister intermediate2(Rl.value().rows, Rl.value().cols);
    cv::bitwise_not(Rx.value(), intermediate.value());
    DigitalBus::AND_MASKED(intermediate2, Ry, intermediate, FLAG);
    DigitalBus::AND_MASKED(intermediate, Rz, Rx, FLAG);
    DigitalBus::OR_MASKED(Rl, intermediate, intermediate2, FLAG);
}

void DigitalBus::CLR_IF_MASKED(DigitalRegister &Rl, DigitalRegister &Rx, DigitalRegister &FLAG) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalRegister intermediate(Rl.value().rows, Rl.value().cols);
    DigitalBus::NOT_MASKED(intermediate, Rl, FLAG);
    DigitalBus::NOR_MASKED(Rl, intermediate, Rx, FLAG);
}
