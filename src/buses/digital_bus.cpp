//
// Created by jm1417 on 28/01/2021.
//

#include "digital_bus.h"

void DigitalBus::OR(DREG &d, DREG &d0, DREG &d1, DREG &FLAG) {
    // d := d0 OR d1
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
}

void DigitalBus::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &FLAG) {
    // d := d0 OR d1 OR d2
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d2.value(), d.value(), FLAG.value());
}

void DigitalBus::OR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3, DREG &FLAG) {
    // d := d0 OR d1 OR d2 OR d3
    cv::bitwise_or(d0.value(), d1.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d2.value(), d.value(), FLAG.value());
    cv::bitwise_or(d.value(), d3.value(), d.value(), FLAG.value());
}

void DigitalBus::NOT(DREG &d, DREG &d0, DREG &FLAG) {
    // d := NOT d0
    cv::bitwise_not(d0.value(), d.value(), FLAG.value());
}

void DigitalBus::NOR(DREG &d, DREG &d0, DREG &d1, DREG &FLAG) {
    // d := NOT(d0 OR d1)
    DigitalBus::OR(d, d0, d1, FLAG);
    DigitalBus::NOT(d, FLAG);
}

void DigitalBus::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &FLAG) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::OR(d, d0, d1, d2,FLAG);
    DigitalBus::NOT(d, FLAG);
}

void DigitalBus::NOR(DREG &d, DREG &d0, DREG &d1, DREG &d2, DREG &d3, DREG &FLAG) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::OR(d, d0, d1, d2,d3, FLAG);
    DigitalBus::NOT(d, FLAG);
}

void DigitalBus::NOT(DREG &Rl, DREG &FLAG) {
    // Rl := NOT Rl
    cv::bitwise_not(Rl.value(), Rl.value(), FLAG.value());
}

void DigitalBus::OR(DREG &Rl, DREG &Rx, DREG &FLAG) {
    // Rl := Rl OR Rx
    DigitalBus::OR(Rl, Rl, Rx, FLAG);
}

void DigitalBus::NOR(DREG &Rl, DREG &Rx, DREG &FLAG) {
    // Rl := Rl NOR Rx
    DigitalBus::NOR(Rl, Rl, Rx, FLAG);
}

void DigitalBus::AND(DREG &Ra, DREG &Rx, DREG &Ry, DREG &FLAG) {
    // Ra := Rx AND Ry
    cv::bitwise_and(Rx.value(), Ry.value(), Ra.value(), FLAG.value());
}

void DigitalBus::NAND(DREG &Ra, DREG &Rx, DREG &Ry, DREG &FLAG) {
    // Ra = NOT(Rx AND Ry)
    DigitalBus::AND(Ra, Rx, Ry, FLAG);
    DigitalBus::NOT(Ra, FLAG);
}

void DigitalBus::IMP(DREG &Rl, DREG &Rx, DREG &Ry, DREG &FLAG) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    DREG intermediate;
    DigitalBus::NOT(intermediate, Ry, FLAG);
    DigitalBus::OR(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::NIMP(DREG &Rl, DREG &Rx, DREG &Ry, DREG &FLAG) {
    // Rl := Rx NIMP Ry
    DREG intermediate;
    DigitalBus::NOT(intermediate, FLAG);
    DigitalBus::NOR(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::XOR(DREG &Rl, DREG &Rx, DREG &Ry, DREG &FLAG) {
    // Rl := Rx XOR Ry
    cv::bitwise_xor(Rx.value(), Ry.value(), Rl.value(), FLAG.value());
}

void DigitalBus::MOV(DREG &d, DREG &d0, DREG &FLAG) {
    // d := d0
    cv::copyTo(d0.value(), d.value(), FLAG.value());
}

void DigitalBus::MUX(DREG &Rl, DREG &Rx, DREG &Ry, DREG &Rz, DREG &FLAG) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    // R1 = (Ry.~Rx) + (Rz.Rx)
    DREG intermediate;
    DREG intermediate2;
    cv::bitwise_not(Rx.value(), intermediate.value());
    DigitalBus::AND(intermediate2, Ry, intermediate, FLAG);
    DigitalBus::AND(intermediate, Rz, Rx, FLAG);
    DigitalBus::OR(Rl, intermediate, intermediate2, FLAG);
}

void DigitalBus::CLR_IF(DREG &Rl, DREG &Rx, DREG &FLAG) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DREG intermediate;
    DigitalBus::NOT(intermediate, Rl);
    DigitalBus::NOR(Rl, intermediate, Rx);
}
