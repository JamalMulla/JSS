//
// Created by jm1417 on 22/12/2020.
//

#include "ProcessingElement.h"

// Analog Register Transfer
void ProcessingElement::bus(AREG a) {
    //a = 0 + error
    a.set(0);
}

void ProcessingElement::bus(AREG a, AREG a0) {
    //a = -a0 + error
    a.set(-a0.get());
}

void ProcessingElement::bus(AREG a, AREG a0, AREG a1) {
    //a = -(a0 + a1) + error
    a.set(-(a0.get() + a1.get()));
}

void ProcessingElement::bus(AREG a, AREG a0, AREG a1, AREG a2) {
    //a = -(a0 + a1 + a2) + error
    a.set(-(a0.get() + a1.get() + a2.get()));
}

void ProcessingElement::bus(AREG a, AREG a0, AREG a1, AREG a2, AREG a3) {
    //a = -(a0 + a1 + a2 + a3) + error
    a.set(-(a0.get() + a1.get() + a2.get() + a3.get()));
}

void ProcessingElement::bus2(AREG a, AREG b) {
    //a,b = 0 + error
    a.set(0);
    b.set(0);
}

void ProcessingElement::bus2(AREG a, AREG b, AREG a0) {
    //a,b = -0.5*a0 + error + noise
    double val = -0.5 * a0.get();
    a.set(val);
    b.set(val);
}

void ProcessingElement::bus2(AREG a, AREG b, AREG a0, AREG a1) {
    //a,b = -0.5*(a0 + a1) + error + noise
    double val = -0.5 * (a0.get() + a1.get());
    a.set(val);
    b.set(val);
}

void ProcessingElement::bus3(AREG a, AREG b, AREG c, AREG a0) {
    //a,b,c = -0.33*a0 + error + noise
    double val = -0.33 * a0.get();
    a.set(val);
    b.set(val);
    c.set(val);
}

void ProcessingElement::where(AREG a) {
    //FLAG := a > 0.
    this->FLAG = a.get() > 0;
}

void ProcessingElement::where(AREG a0, AREG a1) {
    //FLAG := (a0 + a1) > 0.
    this->FLAG = (a0.get() + a1.get()) > 0;
}

void ProcessingElement::where(AREG a0, AREG a1, AREG a2) {
    //FLAG := (a0 + a1 + a2) > 0.
    this->FLAG = (a0.get() + a1.get() + a2.get()) > 0;
}

void ProcessingElement::all() {
    //FLAG := 1.
    this->FLAG = true;
}

void ProcessingElement::mov(AREG y, AREG x0) {
    //y = x0
    if (!FLAG) return;
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

// Analog Arithmetic
void ProcessingElement::res(AREG a) {
    // a = 0
    if (!FLAG) return;
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void ProcessingElement::res(AREG a, AREG b) {
    // a = 0, b := 0
    if (!FLAG) return;
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void ProcessingElement::add(AREG y, AREG x0, AREG x1) {
    // y = x0 + x1
    if (!FLAG) return;
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void ProcessingElement::add(AREG y, AREG x0, AREG x1, AREG x2) {
    // y = x0 + x1 + x2
    if (!FLAG) return;
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void ProcessingElement::sub(AREG y, AREG x0, AREG x1) {
    // y = x0 - x1
    if (!FLAG) return;
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void ProcessingElement::neg(AREG y, AREG x0) {
    // y = -x0
    if (!FLAG) return;
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void ProcessingElement::abs(AREG y, AREG x0) {
    // y = |x0|
    if (!FLAG) return;
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void ProcessingElement::div(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    if (!FLAG) return;
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::div(AREG y0, AREG y1, AREG y2, AREG x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    if (!FLAG) return;
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2,NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::diva(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    if (!FLAG) return;
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void ProcessingElement::divq(AREG y0, AREG x0) {
    // y0 := 0.5*x0 + error
    if (!FLAG) return;
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

// Analog Neighbour Access
//TODO NEWS and XS,XW,etc do we need to move values? how does it work? Neighbours are moved into NEWS
void ProcessingElement::movx(AREG y, AREG x0, const news_t dir) {
    // y = x0_dir
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0);
            break;
        case east:
            this->bus(XW, x0);
            break;
        case south:
            this->bus(XN, x0);
            break;
        case west:
            this->bus(XE, x0);
            break;
        default:
            break;
    }
    this->bus(y, NEWS);
}

void ProcessingElement::mov2x(AREG y, AREG x0, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir (note: this only works when FLAG is "all")
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0);
            break;
        case east:
            this->bus(XW, x0);
            break;
        case south:
            this->bus(XN, x0);
            break;
        case west:
            this->bus(XE, x0);
            break;
        default:
            break;
    }
    switch (dir2) {
        case north:
            this->bus(y, XN);
            break;
        case east:
            this->bus(y, XE);
            break;
        case south:
            this->bus(y, XS);
            break;
        case west:
            this->bus(y, XW);
            break;
        default:
            break;
    }
}

void ProcessingElement::addx(AREG y, AREG x0, AREG x1, const news_t dir) {
    // y = x0_dir + x1_dir
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0, x1);
            break;
        case east:
            this->bus(XW, x0, x1);
            break;
        case south:
            this->bus(XN, x0, x1);
            break;
        case west:
            this->bus(XE, x0, x1);
            break;
        default:
            break;
    }
    this->bus(y, NEWS);
}

void ProcessingElement::add2x(AREG y, AREG x0, AREG x1, const news_t dir, const news_t dir2) {
    // y = x0_dir_dir2 + x1_dir_dir2
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0, x1);
            break;
        case east:
            this->bus(XW, x0, x1);
            break;
        case south:
            this->bus(XN, x0, x1);
            break;
        case west:
            this->bus(XE, x0, x1);
            break;
        default:
            break;
    }
    switch (dir2) {
        case north:
            this->bus(y, XN);
            break;
        case east:
            this->bus(y, XE);
            break;
        case south:
            this->bus(y, XS);
            break;
        case west:
            this->bus(y, XW);
            break;
        default:
            break;
    }
}

void ProcessingElement::subx(AREG y, AREG x0, const news_t dir, AREG x1) {
    // y = x0_dir - x1
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0);
            break;
        case east:
            this->bus(XW, x0);
            break;
        case south:
            this->bus(XN, x0);
            break;
        case west:
            this->bus(XE, x0);
            break;
        default:
            break;
    }
    this->bus(y, NEWS, x1);
}

void ProcessingElement::sub2x(AREG y, AREG x0, const news_t dir, const news_t dir2, AREG x1) {
    // y = x0_dir_dir2 - x1
    if (!FLAG) return;
    switch (dir) {
        case north:
            this->bus(XS, x0);
            break;
        case east:
            this->bus(XW, x0);
            break;
        case south:
            this->bus(XN, x0);
            break;
        case west:
            this->bus(XE, x0);
            break;
        default:
            break;
    }
    switch (dir2) {
        case north:
            this->bus(y, XN, x1);
            break;
        case east:
            this->bus(y, XE, x1);
            break;
        case south:
            this->bus(y, XS, x1);
            break;
        case west:
            this->bus(y, XW, x1);
            break;
        default:
            break;
    }
}

// Asynchronized Blur

// Digital Logic Operations
void ProcessingElement::OR(DREG d, DREG d0, DREG d1) {

}

void ProcessingElement::OR(DREG d, DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::OR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::NOT(DREG d, DREG d0) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::NOR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::NOT(DREG Rl) {
    // logic operation Rl := NOT Rl
    if (!FLAG) return;
    Rl.set(not Rl.get());
}

void ProcessingElement::OR(DREG Rl, DREG Rx) {
    // logic operation Rl := Rl OR Rx
    if (!FLAG) return;
    Rl.set(Rl.get() or Rx.get());
}

void ProcessingElement::NOR(DREG Rl, DREG Rx) {
    // logic operation Rl := Rl NOR Rx
    if (!FLAG) return;
    Rl.set(not (Rl.get() or Rx.get()));
}

void ProcessingElement::AND(DREG Ra, DREG Rx, DREG Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    if (!FLAG) return;
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->NOR(Ra, RF, RP);
}

void ProcessingElement::NAND(DREG Ra, DREG Rx, DREG Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    if (!FLAG) return;
    this->SET(RF);
    this->NOT(RP, Rx);
    this->NOT(RF, Ry);
    this->OR(Ra, RF, RP);
}

void ProcessingElement::ANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    if (!FLAG) return;
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, RF, Rb);
}

void ProcessingElement::NANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    if (!FLAG) return;
    this->NOT(RF, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, RF, Rb);
}

void ProcessingElement::IMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    if (!FLAG) return;
    this->NOT(RF, Ry);
    this->OR(RS, Rx, RF);
}

void ProcessingElement::NIMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx NIMP Ry
    if (!FLAG) return;
    this->NOT(RF, Ry);
    this->NOR(RS, Rx, RF);
}

void ProcessingElement::XOR(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx XOR Ry, Rx := *
    if (!FLAG) return;
    this->NOT(RF, Ry);
    this->NOR(Rl, Rx, RF);
    this->NOT(RF, Rx);
    this->NOR(Rx, Ry, RF);
    this->OR(Rl, Rx);
}

// Digital Register Transfer
void ProcessingElement::WHERE(DREG d) {

}

void ProcessingElement::WHERE(DREG d0, DREG d1) {

}

void ProcessingElement::WHERE(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::ALL() {

}

void ProcessingElement::SET(DREG d0) {

}

void ProcessingElement::SET(DREG d0, DREG d1) {

}

void ProcessingElement::SET(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::SET(DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::CLR(DREG d0) {

}

void ProcessingElement::CLR(DREG d0, DREG d1) {

}

void ProcessingElement::CLR(DREG d0, DREG d1, DREG d2) {

}

void ProcessingElement::CLR(DREG d0, DREG d1, DREG d2, DREG d3) {

}

void ProcessingElement::MOV(DREG d, DREG d0) {

}

void ProcessingElement::MUX(DREG Rl, DREG Rx, DREG Ry, DREG Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    if (!FLAG) return;
    this->SET(RF);
    this->MOV(RP, Rz);
    this->MOV(RF, Rx);
    this->MOV(RP, Ry);
    this->MOV(Rl, RP);
}

void ProcessingElement::CLR_IF(DREG Rl, DREG Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    if (!FLAG) return;
    this->NOT(RF, Rl);
    this->NOR(Rl, RF, Rx);
}

void ProcessingElement::REFRESH(DREG Rl) {
    // refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds) without any operations
    //TODO do we need to do anything here? Maybe add a small delay?
}

// Digital Neighbour Access
void ProcessingElement::DNEWS0(DREG d, DREG d0) {

}

void ProcessingElement::DNEWS1(DREG d, DREG d0) {

}

void ProcessingElement::DNEWS(DREG Ra, DREG Rx, const int dir, const bool boundary) {
    if (!FLAG) return;
    this->CLR(RS, RW, RN, RE);
    // set multiple dreg (upto 4) can be done via one icw
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

// Digital Propagation
void ProcessingElement::PROP0() {
    // async-propagation on RP, masked by RF
}

void ProcessingElement::PROP1() {

}









