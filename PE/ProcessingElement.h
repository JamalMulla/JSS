//
// Created by jm1417 on 22/12/2020.
//

#ifndef SIMULATOR_PROCESSINGELEMENT_H
#define SIMULATOR_PROCESSINGELEMENT_H


#include "AREG.h"
#include "DREG.h"

enum news_t { east = 1, west = 2, north = 4, south = 8, alldir = 15 };

class ProcessingElement {
private:
    bool FLAG;
    AREG A, B, C, D, E, F;
    AREG NEWS;
    AREG XN, XE, XS, XW;
    //Old names R0..R12
    DREG RF, RS, RW, RN, RE, S0, S1, S2, S3, S4, S5, S6, RP;
public:
    // Analog Register Transfer
    void bus(AREG a);
    void bus(AREG a, AREG a0);
    void bus(AREG a, AREG a0, AREG a1);
    void bus(AREG a, AREG a0, AREG a1, AREG a2);
    void bus(AREG a, AREG a0, AREG a1, AREG a2, AREG a3);
    void bus2(AREG a, AREG b);
    void bus2(AREG a, AREG b, AREG a0);
    void bus2(AREG a, AREG b, AREG a0, AREG a1);
    void bus3(AREG a, AREG b, AREG c, AREG a0);
    void where(AREG a);
    void where(AREG a0, AREG a1);
    void where(AREG a0, AREG a1, AREG a2);
    void all();
    void mov(AREG y, AREG x0);

    // Analog Arithmetic
    void res(AREG a);
    void res(AREG a,AREG b);
    void add(AREG y, AREG x0, AREG x1);
    void add(AREG y, AREG x0, AREG x1, AREG x2);
    void sub(AREG y, AREG x0, AREG x1);
    void neg(AREG y, AREG x0);
    void abs(AREG y, AREG x0);
    void div(AREG y0, AREG y1, AREG y2);
    void div(AREG y0, AREG y1, AREG y2, AREG x0);
    void diva(AREG y0, AREG y1, AREG y2);
    void divq(AREG y0, AREG x0);

    // Analog Neighbour Access
    void movx(AREG y, AREG x0, const news_t dir);
    void mov2x(AREG y, AREG x0, const news_t dir, const news_t dir2);
    void addx(AREG y, AREG x0, AREG x1, const news_t dir);
    void add2x(AREG y, AREG x0, AREG x1, const news_t dir, const news_t dir2);
    void subx(AREG y, AREG x0, const news_t dir, AREG x1);
    void sub2x(AREG y, AREG x0, const news_t dir, const news_t dir2, AREG x1);

    // Asynchronized Blur

    // Digital Logic Operations
    void OR(DREG d, DREG d0, DREG d1);
    void OR(DREG d, DREG d0, DREG d1, DREG d2);
    void OR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3);
    void NOT(DREG d, DREG d0);
    void NOR(DREG d, DREG d0, DREG d1);
    void NOR(DREG d, DREG d0, DREG d1, DREG d2);
    void NOR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3);
    void NOT(DREG Rl);
    void OR(DREG Rl, DREG Rx);
    void NOR(DREG Rl, DREG Rx);
    void AND(DREG Ra, DREG Rx, DREG Ry);
    void NAND(DREG Ra, DREG Rx, DREG Ry);
    void ANDX(DREG Ra, DREG Rb, DREG Rx);
    void NANDX(DREG Ra, DREG Rb, DREG Rx);
    void IMP(DREG Rl, DREG Rx, DREG Ry);
    void NIMP(DREG Rl, DREG Rx, DREG Ry);
    void XOR(DREG Rl, DREG Rx, DREG Ry);

    // Digital Register Transfer
    void WHERE(DREG d);
    void WHERE(DREG d0, DREG d1);
    void WHERE(DREG d0, DREG d1, DREG d2);
    void ALL();
    void SET(DREG d0);
    void SET(DREG d0, DREG d1);
    void SET(DREG d0, DREG d1, DREG d2);
    void SET(DREG d0, DREG d1, DREG d2, DREG d3);
    void CLR(DREG d0);
    void CLR(DREG d0, DREG d1);
    void CLR(DREG d0, DREG d1, DREG d2);
    void CLR(DREG d0, DREG d1, DREG d2, DREG d3);
    void MOV(DREG d, DREG d0);
    void MUX(DREG Rl, DREG Rx, DREG Ry, DREG Rz);
    void CLR_IF(DREG Rl, DREG Rx);
    void REFRESH(DREG Rl);

    // Digital Neighbour Access
    void DNEWS0(DREG d, DREG d0);
    void DNEWS1(DREG d, DREG d0);
    void DNEWS(DREG Ra, DREG Rx, const int dir, const bool boundary);

    // Digital Propagation
    void PROP0();
    void PROP1();

};


#endif //SIMULATOR_PROCESSINGELEMENT_H
