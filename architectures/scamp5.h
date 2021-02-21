//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_SCAMP5_H
#define SIMULATOR_SCAMP5_H

#include <opencv2/opencv.hpp>
#include "../core/src/base/processing_element.h"
#include "../core/src/base/array.h"

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
    CycleCounter cycles;
private:
    ProcessingElement pe = ProcessingElement::builder{}
                            .with_rows(SCAMP_WIDTH)
                            .with_cols(SCAMP_HEIGHT)
                            .with_analogue_registers(ANALOGUE_REGISTERS)
                            .with_digital_registers(DIGITAL_REGISTERS)
                            .build();
    Array array = Array(SCAMP_HEIGHT, SCAMP_WIDTH, pe);
public:
    //Analogue registers
    AREG& PIX = array.pe.analogue_registers[0]("PIX");
    AREG& IN = array.pe.analogue_registers[1]("IN");
    AREG& NEWS = array.pe.analogue_registers[2]("NEWS");
    AREG& A = array.pe.analogue_registers[3]("A");
    AREG& B = array.pe.analogue_registers[4]("B");
    AREG& C = array.pe.analogue_registers[5]("C");
    AREG& D = array.pe.analogue_registers[6]("D");
    AREG& E = array.pe.analogue_registers[7]("E");
    AREG& F = array.pe.analogue_registers[8]("F");
    AREG& XE = array.pe.analogue_registers[9]("XE");
    AREG& XW = array.pe.analogue_registers[10]("XW");
    AREG& XN = array.pe.analogue_registers[11]("XN");
    AREG& XS = array.pe.analogue_registers[12]("XS");

    //Digital registers
    DREG& FLAG = array.pe.digital_registers[0]("FLAG");
    DREG& RF = array.pe.digital_registers[1]("RF");
    DREG& RS = array.pe.digital_registers[2]("RS");
    DREG& RW = array.pe.digital_registers[3]("RW");
    DREG& RN = array.pe.digital_registers[4]("RN");
    DREG& RE = array.pe.digital_registers[5]("RE");
    DREG& S0 = array.pe.digital_registers[6]("S0");
    DREG& S1 = array.pe.digital_registers[7]("S1");
    DREG& S2 = array.pe.digital_registers[8]("S2");
    DREG& S3 = array.pe.digital_registers[9]("S3");
    DREG& S4 = array.pe.digital_registers[10]("S4");
    DREG& S5 = array.pe.digital_registers[11]("S5");
    DREG& S6 = array.pe.digital_registers[12]("S6");
    DREG& RP = array.pe.digital_registers[13]("RP");

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
    void bus(AREG& a,  AREG& a0);
    void bus(AREG& a,  AREG& a0,  AREG& a1);
    void bus(AREG& a,  AREG& a0,  AREG& a1,  AREG& a2);
    void bus(AREG& a,  AREG& a0,  AREG& a1,  AREG& a2,  AREG& a3);
    void bus2(AREG& a, AREG& b);
    void bus2(AREG& a, AREG& b,  AREG& a0);
    void bus2(AREG& a, AREG& b,  AREG& a0,  AREG& a1);
    void bus3(AREG& a, AREG& b, AREG& c,  AREG& a0);
    void where( AREG& a);
    void where( AREG& a0,  AREG& a1);
    void where( AREG& a0,  AREG& a1,  AREG& a2);
    void all();
    void mov(AREG& y,  AREG& x0);

    // Analog Arithmetic
    void res(AREG& a);
    void res(AREG& a, AREG& b);
    void add(AREG& y,  AREG& x0,  AREG& x1);
    void add(AREG& y,  AREG& x0,  AREG& x1,  AREG& x2);
    void sub(AREG& y,  AREG& x0,  AREG& x1);
    void neg(AREG& y,  AREG& x0);
    void abs(AREG& y,  AREG& x0);
    void div(AREG& y0, AREG& y1, AREG& y2);
    void div(AREG& y0, AREG& y1, AREG& y2, AREG& x0);
    void diva(AREG& y0, AREG& y1, AREG& y2);
    void divq(AREG& y0, AREG& x0);

    // Analog Neighbour Access
    void movx(AREG& y,  AREG& x0, news_t dir);
    void mov2x(AREG& y,  AREG& x0, news_t dir, news_t dir2);
    void addx(AREG& y,  AREG& x0,  AREG& x1, news_t dir);
    void add2x(AREG& y,  AREG& x0,  AREG& x1, news_t dir, news_t dir2);
    void subx(AREG& y,  AREG& x0, news_t dir,  AREG& x1);
    void sub2x(AREG& y,  AREG& x0, news_t dir, news_t dir2,  AREG& x1);

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
    void print_stats(const CycleCounter& counter);

};




#define scamp5_kernel_begin() { using namespace SCAMP_NAME;
#define scamp5_kernel_end()    }

#define scamp5_dynamic_kernel_begin() { using namespace SCAMP5_PE; using namespace scamp5_kernel_api; scamp5_kernel kn; kn.begin();
#define scamp5_dynamic_kernel_end()   kn.end(); }


#endif //SIMULATOR_SCAMP5_H
