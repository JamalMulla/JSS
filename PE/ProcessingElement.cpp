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
    this->bus(NEWS, x0);
    this->bus(y, NEWS);
}

// Analog Arithmetic
void ProcessingElement::res(AREG a) {
    // a = 0
    this->bus(NEWS);
    this->bus(a, NEWS);
}

void ProcessingElement::res(AREG a, AREG b) {
    // a = 0, b := 0
    this->bus(NEWS);
    this->bus(a, NEWS);
    this->bus(b, NEWS);
}

void ProcessingElement::add(AREG y, AREG x0, AREG x1) {
    // y = x0 + x1
    this->bus(NEWS, x0, x1);
    this->bus(y, NEWS);
}

void ProcessingElement::add(AREG y, AREG x0, AREG x1, AREG x2) {
    // y = x0 + x1 + x2
    this->bus(NEWS, x0, x1, x2);
    this->bus(y, NEWS);
}

void ProcessingElement::sub(AREG y, AREG x0, AREG x1) {
    // y = x0 - x1
    this->bus(NEWS, x0);
    this->bus(y, NEWS, x1);
}

void ProcessingElement::neg(AREG y, AREG x0) {
    // y = -x0
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
}

void ProcessingElement::abs(AREG y, AREG x0) {
    // y = |x0|
    this->bus(NEWS);
    this->bus(y, NEWS, x0);
    this->bus(NEWS, x0);
    this->where(x0);
    this->bus(y, NEWS);
    this->all();
}

void ProcessingElement::div(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y2; y1 := -0.5*y2 + error, y2 := y2 + error
    this->bus2(y0, y1, y2);
    this->bus(NEWS, y2, y1);
    this->bus(y2, NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::div(AREG y0, AREG y1, AREG y2, AREG x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0 + error, y2 := x0 + error
    this->bus2(y0, y1, x0);
    this->bus(NEWS, x0, y1);
    this->bus(y2,NEWS, y0);
    this->bus2(y0, y1, y2);
    this->bus(y0, y1);
}

void ProcessingElement::diva(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0 + error, y2 := -0.5*y0 + error
    this->bus2(y1, y2, y0);
    this->bus(NEWS, y1, y0);
    this->bus(y0, NEWS, y2);
    this->bus2(y1, y2, y0);
    this->bus(y0, y1);
}

void ProcessingElement::divq(AREG y0, AREG x0) {
    // y0 := 0.5*x0 + error
    this->bus2(y0, NEWS, x0);
    this->bus(y0, NEWS);
}

// Analog Neighbour Access
//TODO NEWS and XS,XW,etc do we need to move values? how does it work?
void ProcessingElement::movx(AREG y, AREG x0, const news_t dir) {
    // y = x0_dir
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




