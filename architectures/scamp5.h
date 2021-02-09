//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_SCAMP5_H
#define SIMULATOR_SCAMP5_H

#include <opencv2/opencv.hpp>
#include "../src/base/processing_element.h"
#include "../src/base/array.h"

enum news_t { east = 1, west = 2, north = 4, south = 8, alldir = 15 };

typedef AnalogueRegister AREG;
typedef DigitalRegister DREG;

#define RS  R1
#define RW  R2
#define RN  R3
#define RE  R4
#define S0  R5
#define S1  R6
#define S2  R7
#define S3  R8
#define S4  R9
#define S5  R10
#define S6  R11
#define RP  R12
#define RF  R0

class SCAMP5 {
public:
    const int SCAMP_HEIGHT = 256;
    const int SCAMP_WIDTH = 256;
    const int ANALOGUE_REGISTERS = 13;
    const int DIGITAL_REGISTERS = 14;
private:
    ProcessingElement pe = ProcessingElement(SCAMP_HEIGHT, SCAMP_WIDTH, ANALOGUE_REGISTERS, DIGITAL_REGISTERS);
    Array array = Array(SCAMP_HEIGHT, SCAMP_WIDTH, pe);
public:
    //Analogue registers
    AREG& PIX = array.pe.analogue_registers[0];
    AREG& IN = array.pe.analogue_registers[1];
    AREG& NEWS = array.pe.analogue_registers[2];
    AREG& A = array.pe.analogue_registers[3];
    AREG& B = array.pe.analogue_registers[4];
    AREG& C = array.pe.analogue_registers[5];
    AREG& D = array.pe.analogue_registers[6];
    AREG& E = array.pe.analogue_registers[7];
    AREG& F = array.pe.analogue_registers[8];
    AREG& XE = array.pe.analogue_registers[9];
    AREG& XW = array.pe.analogue_registers[10];
    AREG& XN = array.pe.analogue_registers[11];
    AREG& XS = array.pe.analogue_registers[12];

    //Digital registers
    DREG& FLAG = array.pe.digital_registers[0];
    DREG& RF = array.pe.digital_registers[1];
    DREG& RS = array.pe.digital_registers[2];
    DREG& RW = array.pe.digital_registers[3];
    DREG& RN = array.pe.digital_registers[4];
    DREG& RE = array.pe.digital_registers[5];
    DREG& S0 = array.pe.digital_registers[6];
    DREG& S1 = array.pe.digital_registers[7];
    DREG& S2 = array.pe.digital_registers[8];
    DREG& S3 = array.pe.digital_registers[9];
    DREG& S4 = array.pe.digital_registers[10];
    DREG& S5 = array.pe.digital_registers[11];
    DREG& S6 = array.pe.digital_registers[12];
    DREG& RP = array.pe.digital_registers[13];


    SCAMP5();

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
    void blurset();
    void blur(AREG& a, AREG& a0);
    void blurh(AREG& a, AREG& a0);
    void blurv(AREG& a, AREG& a0);
    void gauss(AREG& y, AREG& x, int iterations);
    void gaussh(AREG& y, AREG& x, int iterations);
    void gaussv(AREG& y, AREG& x, int iterations);
    void newsblur(AREG& y, AREG& x, int iterations);
    void newsblurh(AREG& y, AREG& x, int iterations);
    void newsblurv(AREG& y, AREG& x, int iterations);

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

    // Vision system functions
    //   Analogue
    void scamp5_get_image(AREG& yf, AREG& yh, int gain=1);
    void scamp5_in(AREG& areg, int8_t value, AREG* temp = nullptr);
    void scamp5_load_in(AREG& areg, int8_t value, AREG* temp = nullptr);
    void scamp5_load_in(int8_t value);
    void scamp5_load_dac(AREG& areg, int16_t value, AREG& temp);
    void scamp5_load_dac(int16_t value);
    void scamp5_shift(AREG& areg, int h, int v);
    void scamp5_diffuse();
    void scamp5_read_areg();
    void scamp5_global_sum_16();
    void scamp5_global_sum_64();
    void scamp5_global_sum_fast();

    //    Digital
    void scamp5_global_or();
    void scamp5_global_count();
    void scamp5_flood();
    void scamp5_load_point();
    void scamp5_load_rect();
    void scamp5_load_pattern();
    void scamp5_select_point();
    void scamp5_select_rect();
    void scamp5_select_pattern();
    void scamp5_draw_begin();
    void scamp5_draw_end();
    void scamp5_draw_pixel();
    void scamp5_draw_point();
    void scamp5_draw_rect();
    void scamp5_draw_line();
    void scamp5_draw_circle();
    void scamp5_draw_negate();

    //    Image Readout
    void scamp5_scan_areg();
    void scamp5_scan_areg_8x8();
    void scamp5_scan_areg_mean_8x8();
    void scamp5_scan_dreg();
    void scamp5_display_image();
    void scamp5_display_image_patch();
    void scamp5_output_image();
    void scamp5_output_bitstack_begin();
    void scamp5_output_bitstack_bit();
    void scamp5_output_bitstack_end();


    //    Events Readout
    void scamp5_scan_events();
    void scamp5_scan_boundingbox();
    void scamp5_display_events();
    void scamp5_display_boundingbox();
    void scamp5_output_events();
    void scamp5_output_boundingbox();

    //    Other Functions
    void scamp5_bind_io_agent();
    void scamp5_get_io_agent();


    // Simulator specific methods
    void print_stats();
};




#define scamp5_kernel_begin() { using namespace SCAMP_NAME;
#define scamp5_kernel_end()    }

#define scamp5_dynamic_kernel_begin() { using namespace SCAMP5_PE; using namespace scamp5_kernel_api; scamp5_kernel kn; kn.begin();
#define scamp5_dynamic_kernel_end()   kn.end(); }


#endif //SIMULATOR_SCAMP5_H
