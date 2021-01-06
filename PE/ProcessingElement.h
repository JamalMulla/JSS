//
// Created by jm1417 on 22/12/2020.
//

#ifndef SIMULATOR_PROCESSINGELEMENT_H
#define SIMULATOR_PROCESSINGELEMENT_H

#include <opencv2/opencv.hpp>

enum news_t { east = 1, west = 2, north = 4, south = 8, alldir = 15 };

typedef cv::Mat AREG;
typedef cv::Mat DREG;

#define SCAMP_HEIGHT 256
#define SCAMP_WIDTH 256
#define MAT_TYPE CV_8S

#define R1 RS
#define R2 RW
#define R3 RN
#define R4 RE
#define R5 S0
#define R6 S1
#define R7 S2
#define R8 S3
#define R9 S4
#define R10 S5
#define R11 S6
#define R12 RP
#define R0 RF

class ProcessingElement {
private:
public:
    cv::Mat FLAG;
    AREG PIX, IN;
    AREG A, B, C, D, E, F;
    AREG NEWS;
    AREG XN, XE, XS, XW;
    //Old names R0..R12
    DREG RF, RS, RW, RN, RE, S0, S1, S2, S3, S4, S5, S6, RP;
    //Constructor
    ProcessingElement();

    //Helpers
    /*Pushes value from register to NEWS of dir PE. Fills with 0s*/
    void pushToNews(AREG& src, news_t dir);
    /*Pulls value from NEWS of dir to current PE. Fills with 0s*/
    void pullFromNews(AREG& dst, news_t dir);

    // Misc
    void nop();

    // Image Capturing
    void rpix();
    void get_image(AREG& y);
    void get_image(AREG& y, AREG& h);
    void respix();
    void respix(AREG& y);
    void getpix(AREG& y, AREG& pix_res);
    void getpix(AREG& y, AREG& h, AREG& pix_res);

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
    void OR(DREG& d, DREG& d0, DREG& d1);
    void OR(DREG& d, DREG& d0, DREG& d1, DREG& d2);
    void OR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3);
    void NOT(DREG& d, DREG& d0);
    void NOR(DREG& d, DREG& d0, DREG& d1);
    void NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2);
    void NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3);
    void NOT(DREG& Rl);
    void OR(DREG& Rl, DREG& Rx);
    void NOR(DREG& Rl, DREG& Rx);
    void AND(DREG& Ra, DREG& Rx, DREG& Ry);
    void NAND(DREG& Ra, DREG& Rx, DREG& Ry);
    void ANDX(DREG& Ra, DREG& Rb, DREG& Rx);
    void NANDX(DREG& Ra, DREG& Rb, DREG& Rx);
    void IMP(DREG& Rl, DREG& Rx, DREG& Ry);
    void NIMP(DREG& Rl, DREG& Rx, DREG& Ry);
    void XOR(DREG& Rl, DREG& Rx, DREG& Ry);

    // Digital Register Transfer
    void WHERE(DREG& d);
    void WHERE(DREG& d0, DREG& d1);
    void WHERE(DREG& d0, DREG& d1, DREG& d2);
    void ALL();
    void SET(DREG& d0);
    void SET(DREG& d0, DREG& d1);
    void SET(DREG& d0, DREG& d1, DREG& d2);
    void SET(DREG& d0, DREG& d1, DREG& d2, DREG& d3);
    void CLR(DREG& d0);
    void CLR(DREG& d0, DREG& d1);
    void CLR(DREG& d0, DREG& d1, DREG& d2);
    void CLR(DREG& d0, DREG& d1, DREG& d2, DREG& d3);
    void MOV(DREG& d, DREG& d0);
    void MUX(DREG& Rl, DREG& Rx, DREG& Ry, DREG& Rz);
    void CLR_IF(DREG& Rl, DREG& Rx);
    void REFRESH(DREG& Rl);

    // Digital Neighbour Access
    void DNEWS0(DREG& d, DREG& d0);
    void DNEWS1(DREG& d, DREG& d0);
    void DNEWS(DREG& Ra, DREG& Rx, int dir, bool boundary);

    // Digital Propagation
    void PROP0();
    void PROP1();
};


#endif //SIMULATOR_PROCESSINGELEMENT_H
