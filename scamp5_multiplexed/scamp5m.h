//
// Created by jm1417 on 05/05/2021.
//

#ifndef SIMULATOR_SCAMP5M_H
#define SIMULATOR_SCAMP5M_H

#include <simulator/adc/adc.h>
#include <simulator/alu/alu.h>

#include <opencv4/opencv2/opencv.hpp>
#include <rttr/type>

#include "simulator/base/architecture.h"
#include "simulator/pe/processing_element.h"

enum AREG {
    PIX = 0,
    IN = 8,
    NEWS = 16,
    A = 24,
    B = 32,
    C = 40,
    D = 48,
    E = 56,
    F = 64
};

enum DREG {
    FLAG = 72,
    SELECT = 73,
    RECT = 74,
    R1 = 75,
    R2 = 76,
    R3 = 77,
    R4 = 78,
    R5 = 79,
    R6 = 80,
    R7 = 81,
    R8 = 82,
    R9 = 83,
    R10 = 84,
    R11 = 85,
    R12 = 86,
    R0 = 87
};

/*Multiplexed architecture*/

class SCAMP5M : public Architecture {

   protected:
    int rows_;
    int cols_;
    int row_stride_ = 1;
    int col_stride_ = 1;
    Origin origin_;
    int num_pes = 0; // number of PEs in total
    int num_pixels = 0; // number of pixels in

   public:

    std::shared_ptr<ProcessingElement> pe;
    std::shared_ptr<ALU> alu;
    std::shared_ptr<Dram> dram;
    std::shared_ptr<ADC> adc;
    DREG scratch = R0;

    SCAMP5M() = default;
    void init();
    rttr::variant config_converter(json& j);
    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_origin(Origin origin);
    void set_config(std::shared_ptr<Config> config);
    void set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components);

    // Misc
    void nop();

    // Image Capturing
    void rpix();

    void get_image(AREG y);

    void get_image(AREG y, AREG h);


//    void bus(AREG a);
//
//    void bus(AREG a, AREG a0);
//
//    void bus(AREG a, AREG a0, AREG a1);
//
//    void bus(AREG a, AREG a0, AREG a1, AREG a2);
//
//    void bus(AREG a, AREG a0, AREG a1, AREG a2, AREG a3);
//
//    void bus2(AREG a, AREG b);
//
//    void bus2(AREG a, AREG b, AREG a0);
//
//    void bus2(AREG a, AREG b, AREG a0, AREG a1);
//
//    void bus3(AREG a, AREG b, AREG c, AREG a0);

    void where(AREG a);

    void where(AREG a0, AREG a1);

    void where(AREG a0, AREG a1, AREG a2);

    void all();

    void mov(AREG y, AREG x0);

    // Analog Arithmetic
    void res(AREG a);

    void res(AREG a, AREG b);

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
    void movx(AREG y, AREG x0, news_t dir);

    void mov2x(AREG y, AREG x0, news_t dir, news_t dir2);

    void addx(AREG y, AREG x0, AREG x1, news_t dir);

    void add2x(AREG y, AREG x0, AREG x1, news_t dir, news_t dir2);

    void subx(AREG y, AREG x0, news_t dir, AREG x1);

    void sub2x(AREG y, AREG x0, news_t dir, news_t dir2, AREG x1);

    // Asynchronized Blur
//    void blurset();
//
//    void blur(AREG a, AREG a0);
//
//    void blurh(AREG a, AREG a0);
//
//    void blurv(AREG a, AREG a0);
//
//    void gauss(AREG y, AREG x, int iterations = 3);
//
//    void gaussh(AREG y, AREG x, int iterations = 3);
//
//    void gaussv(AREG y, AREG x, int iterations = 3);
//
//    void newsblur(AREG y, AREG x, int iterations = 1);
//
//    void newsblurh(AREG y, AREG x, int iterations = 1);
//
//    void newsblurv(AREG y, AREG x, int iterations = 1);

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

    // Digital Neighbour Access
    void DNEWS0(DREG d, DREG d0);

    void DNEWS1(DREG d, DREG d0);

    void DNEWS(DREG Ra, DREG Rx, int dir, bool boundary);


    // Vision system functions
    //   Analogue

    // todo overload all the default ones and add to registration
    void scamp5_in(AREG areg, int16_t value);
    void scamp5_in(AREG areg, int16_t value, AREG temp);

    void scamp5_load_in(AREG areg, int8_t value, AREG temp);
    void scamp5_load_in(AREG areg, int8_t value);

    void scamp5_load_in(int8_t value);

    void scamp5_load_dac(AREG areg, uint16_t value, AREG temp);
    void scamp5_load_dac(AREG areg, uint16_t value);

    void scamp5_load_dac(uint16_t value);

//    void scamp5_diffuse(AREG target, int iterations);
//    void scamp5_diffuse(AREG target, int iterations, bool vertical);
//    void scamp5_diffuse(AREG target, int iterations, bool vertical, bool horizontal);
//    void scamp5_diffuse(AREG target, int iterations, bool vertical, bool horizontal, std::shared_ptr<AREG>& t0);

    uint8_t scamp5_read_areg(AREG areg, uint8_t r, uint8_t c);

    int scamp5_global_or(DREG dreg, uint8_t r = 0, uint8_t c = 0, uint8_t rx = 255, uint8_t cx = 255);

    int scamp5_global_count(DREG dreg, AREG temp, int options = 0);

    void scamp5_load_point(DREG dr, uint8_t r, uint8_t c);

    void scamp5_load_rect(DREG dr, uint8_t r0, uint8_t c0, uint8_t r1,
                          uint8_t c1);

    void scamp5_load_pattern(DREG dr, uint8_t r, uint8_t c, uint8_t rx,
                             uint8_t cx);

    void scamp5_select_point(uint8_t r, uint8_t c);

    void scamp5_select_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1);

    void scamp5_select_pattern(uint8_t r, uint8_t c, uint8_t rx, uint8_t cx);

    void scamp5_select_col(uint8_t c);

    void scamp5_select_row(uint8_t r);

    void scamp5_select_colx(uint8_t cx);

    void scamp5_select_rowx(uint8_t rx);

    void scamp5_draw_begin(DREG dr);

    void scamp5_draw_end();

    void scamp5_draw_pixel(uint8_t r, uint8_t c);

    bool scamp5_draw_point(int r, int c);

//    void scamp5_draw_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1);

//    void scamp5_draw_line(int r0, int c0, int r1, int c1, bool repeat = false);

//    void scamp5_draw_circle(int x0, int y0, int radius, bool repeat = false);

    void scamp5_draw_negate();

    //    Image Readout
    void scamp5_scan_areg(AREG areg, uint8_t* buffer, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs, uint8_t cs);

    void scamp5_scan_areg_8x8(AREG areg, uint8_t* result8x8);

    void scamp5_scan_dreg(DREG dreg, uint8_t* mem, uint8_t r0 = 0,
                          uint8_t r1 = 255);

    void scamp5_scan_events(DREG dreg, uint8_t* mem, uint16_t max_num = 1000, uint8_t h_dir = 0, uint8_t v_dir = 0);

    void scamp5_scan_events(DREG dreg, uint8_t* buffer, uint16_t max_num, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs, uint8_t cs);

    void display();

    void viola_jones();

    // Simulator specific methods
    void print_stats(json& config, const std::string& output_path = "");
    RTTR_ENABLE(Architecture);
};

#endif  //SIMULATOR_SCAMP5M_H
