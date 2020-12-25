//
// Created by jm1417 on 22/12/2020.
//

#ifndef SIMULATOR_PROCESSINGELEMENT_H
#define SIMULATOR_PROCESSINGELEMENT_H

#include "opencv2/opencv.hpp"

enum news_t { east = 1, west = 2, north = 4, south = 8, alldir = 15 };

typedef cv::UMat AREG;
typedef cv::UMat DREG;

class ProcessingElement {
private:
    cv::UMat FLAG;
    AREG A, B, C, D, E, F;
    AREG NEWS;
    AREG XN, XE, XS, XW;
    //Old names R0..R12
    DREG RF, RS, RW, RN, RE, S0, S1, S2, S3, S4, S5, S6, RP;
public:
    //Constructor
    ProcessingElement();
    // Analog Register Transfer
    void bus(AREG& a);
    void bus(AREG& a, const AREG& a0);
    void bus(AREG& a, const AREG& a0, const AREG& a1);
    void bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2);
    void bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2, const AREG& a3);
    void bus2(AREG& a, AREG& b);
    void bus2(AREG& a, AREG& b, const AREG& a0);
    void bus2(AREG& a, AREG& b, const AREG& a0, const AREG& a1);
    void bus3(AREG& a, AREG& b, AREG& c, const AREG& a0);
    void where(const AREG& a);
    void where(const AREG& a0, const AREG& a1);
    void where(const AREG& a0, const AREG& a1, const AREG& a2);
    void all();
    void mov(AREG& y, const AREG& x0);

    // Analog Arithmetic
    void res(AREG& a);
    void res(AREG& a, AREG& b);
    void add(AREG& y, const AREG& x0, const AREG& x1);
    void add(AREG& y, const AREG& x0, const AREG& x1, const AREG& x2);
    void sub(AREG& y, const AREG& x0, const AREG& x1);
    void neg(AREG& y, const AREG& x0);
    void abs(AREG& y, const AREG& x0);
    void div(AREG& y0, AREG& y1, AREG& y2);
    void div(AREG& y0, AREG& y1, AREG& y2, const AREG& x0);
    void diva(AREG& y0, AREG& y1, AREG& y2);
    void divq(AREG& y0, const AREG& x0);

    // Analog Neighbour Access
    void movx(AREG& y, const AREG& x0, news_t dir);
    void mov2x(AREG& y, const AREG& x0, news_t dir, news_t dir2);
    void addx(AREG& y, const AREG& x0, const AREG& x1, news_t dir);
    void add2x(AREG& y, const AREG& x0, const AREG& x1, news_t dir, news_t dir2);
    void subx(AREG& y, const AREG& x0, news_t dir, const AREG& x1);
    void sub2x(AREG& y, const AREG& x0, news_t dir, news_t dir2, const AREG& x1);

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
    void DNEWS(DREG Ra, DREG Rx, int dir, bool boundary);

    // Digital Propagation
    void PROP0();
    void PROP1();

    int SCAMP_HEIGHT = 256;
    int SCAMP_WIDTH = 256;
};


#endif //SIMULATOR_PROCESSINGELEMENT_H
