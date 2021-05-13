//
// Created by jm1417 on 12/05/2021.
//

#ifndef SIMULATOR_SCAMP5RMALT_H
#define SIMULATOR_SCAMP5RMALT_H

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

class SCAMP5RMALT : public Architecture {
   RTTR_ENABLE(Architecture);

  private:
   int row_;
   int col_;

   protected:
    int rows_;
    int cols_;
    int row_stride_ = 1;
    int col_stride_ = 1;
    Origin origin_;

   public:

    std::shared_ptr<ProcessingElement> pe;
    std::shared_ptr<ALU> alu;
    std::shared_ptr<Dram> dram;
    std::shared_ptr<ADC> adc;
    DREG scratch = R0;

    SCAMP5RMALT() = default;
    void init();
    rttr::variant config_converter(json& j);
    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_origin(Origin origin);
    void set_config(std::shared_ptr<Config> config);
    void set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components);

    void motion();

    // Misc
    void nop();

    // Image Capturing
    void rpix();


    void get_image(AREG y, AREG h);


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

    // Digital Neighbour Access
    void DNEWS0(DREG d, DREG d0);

    void DNEWS1(DREG d, DREG d0);

    void DNEWS(DREG Ra, DREG Rx, int dir, bool boundary);

    void scamp5_in(AREG areg, int16_t value);
    void scamp5_in(AREG areg, int16_t value, AREG temp);

    uint8_t scamp5_read_areg(AREG areg, uint8_t r, uint8_t c);


    void start();
    void end();
    void display();

    // Simulator specific methods
    void print_stats();

};

#endif  //SIMULATOR_SCAMP5RMALT_H
