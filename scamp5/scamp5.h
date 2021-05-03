//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_SCAMP5_H
#define SIMULATOR_SCAMP5_H

#include <opencv4/opencv2/opencv.hpp>
#include <rttr/type>

#include "simulator/base/architecture.h"
#include "simulator/base/processing_element.h"

enum news_t { east = 1,
              west = 2,
              north = 4,
              south = 8,
              alldir = 15 };

typedef AnalogueRegister AREG;
typedef DigitalRegister DREG;
#define RS R1
#define RW R2
#define RN R3
#define RE R4
#define S0 R5
#define S1 R6
#define S2 R7
#define S3 R8
#define S4 R9
#define S5 R10
#define S6 R11
#define RP R12
#define RF R0

class SCAMP5 : public Architecture {
   protected:
    int rows_;
    int cols_;
    Origin origin_;
    std::shared_ptr<Config> config_;

   public:
    std::shared_ptr<ProcessingElement> pe;
    std::shared_ptr<AREG> intermediate_a;
    std::shared_ptr<AREG> intermediate_a2;
    std::shared_ptr<DREG> intermediate_d;
    std::shared_ptr<DREG> scratch = nullptr;

    void init();
    rttr::variant components_converter(json& j);
    rttr::variant config_converter(json& j);
    void set_rows(int rows);
    void set_cols(int cols);
    void set_origin(Origin origin);
    void set_config(std::shared_ptr<Config> config);
    void set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components);

    // Analogue registers
    //    // TODO make these 4 usable? How should they be handled if they are
    //    used in some instruction? AREG& XE =
    //    array.pe.analogue_registers_[9]("XE"); AREG& XW =
    //    array.pe.analogue_registers_[10]("XW"); AREG& XN =
    //    array.pe.analogue_registers_[11]("XN"); AREG& XS =
    //    array.pe.analogue_registers_[12]("XS");

    // Digital registers
    std::shared_ptr<AREG> PIX;
    std::shared_ptr<AREG> IN;
    std::shared_ptr<AREG> NEWS;
    std::shared_ptr<AREG> A;
    std::shared_ptr<AREG> B;
    std::shared_ptr<AREG> C;
    std::shared_ptr<AREG> D;
    std::shared_ptr<AREG> E;
    std::shared_ptr<AREG> F;

    std::shared_ptr<DREG> FLAG;
    std::shared_ptr<DREG> RF;
    std::shared_ptr<DREG> RS;
    std::shared_ptr<DREG> RW;
    std::shared_ptr<DREG> RN;
    std::shared_ptr<DREG> RE;
    std::shared_ptr<DREG> S0;
    std::shared_ptr<DREG> S1;
    std::shared_ptr<DREG> S2;
    std::shared_ptr<DREG> S3;
    std::shared_ptr<DREG> S4;
    std::shared_ptr<DREG> S5;
    std::shared_ptr<DREG> S6;
    std::shared_ptr<DREG> RP;
    std::shared_ptr<DREG> SELECT;
    std::shared_ptr<DREG> RECT;

    SCAMP5();

    // Misc
    void nop();

    // Image Capturing
    void rpix();

    void get_image(const std::shared_ptr<AREG>& y);

    void get_image(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& h);

    void respix();

    void respix(const std::shared_ptr<AREG>& y);

    void getpix(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& pix_res);

    void getpix(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& h, const std::shared_ptr<AREG>& pix_res);

    void bus(const std::shared_ptr<AREG>& a);

    void bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0);

    void bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1);

    void bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2);

    void bus(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2, const std::shared_ptr<AREG>& a3);

    void bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b);

    void bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& a0);

    void bus2(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1);

    void bus3(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b, const std::shared_ptr<AREG>& c, const std::shared_ptr<AREG>& a0);

    void where(const std::shared_ptr<AREG>& a);

    void where(const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1);

    void where(const std::shared_ptr<AREG>& a0, const std::shared_ptr<AREG>& a1, const std::shared_ptr<AREG>& a2);

    void all();

    void mov(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0);

    // Analog Arithmetic
    void res(const std::shared_ptr<AREG>& a);

    void res(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& b);

    void add(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1);

    void add(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, const std::shared_ptr<AREG>& x2);

    void sub(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1);

    void neg(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0);

    void abs(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0);

    void div(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2);

    void div(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2, const std::shared_ptr<AREG>& x0);

    void diva(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& y1, const std::shared_ptr<AREG>& y2);

    void divq(const std::shared_ptr<AREG>& y0, const std::shared_ptr<AREG>& x0);

    // Analog Neighbour Access
    void movx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir);

    void mov2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, news_t dir2);

    void addx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, news_t dir);

    void add2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, const std::shared_ptr<AREG>& x1, news_t dir, news_t dir2);

    void subx(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, const std::shared_ptr<AREG>& x1);

    void sub2x(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x0, news_t dir, news_t dir2, const std::shared_ptr<AREG>& x1);

    // Asynchronized Blur
    void blurset();

    void blur(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0);

    void blurh(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0);

    void blurv(const std::shared_ptr<AREG>& a, const std::shared_ptr<AREG>& a0);

    void gauss(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 3);

    void gaussh(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 3);

    void gaussv(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 3);

    void newsblur(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 1);

    void newsblurh(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 1);

    void newsblurv(const std::shared_ptr<AREG>& y, const std::shared_ptr<AREG>& x, int iterations = 1);

    // Digital Logic Operations
    void OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1);

    void OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2);

    void OR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3);

    void NOT(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0);

    void NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1);

    void NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2);

    void NOR(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3);

    void NOT(const std::shared_ptr<DREG>& Rl);

    void OR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx);

    void NOR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx);

    void AND(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry);

    void NAND(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry);

    void ANDX(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rb, const std::shared_ptr<DREG>& Rx);

    void NANDX(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rb, const std::shared_ptr<DREG>& Rx);

    void IMP(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry);

    void NIMP(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry);

    void XOR(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry);

    // Digital Register Transfer
    void WHERE(const std::shared_ptr<DREG>& d);

    void WHERE(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1);

    void WHERE(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2);

    void ALL();

    void SET(const std::shared_ptr<DREG>& d0);

    void SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1);

    void SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2);

    void SET(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3);

    void CLR(const std::shared_ptr<DREG>& d0);

    void CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1);

    void CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2);

    void CLR(const std::shared_ptr<DREG>& d0, const std::shared_ptr<DREG>& d1, const std::shared_ptr<DREG>& d2, const std::shared_ptr<DREG>& d3);

    void MOV(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0);

    void MUX(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx, const std::shared_ptr<DREG>& Ry, const std::shared_ptr<DREG>& Rz);

    void CLR_IF(const std::shared_ptr<DREG>& Rl, const std::shared_ptr<DREG>& Rx);

    void REFRESH(const std::shared_ptr<DREG>& Rl);

    // Digital Neighbour Access
    void DNEWS0(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0);

    void DNEWS1(const std::shared_ptr<DREG>& d, const std::shared_ptr<DREG>& d0);

    void DNEWS(const std::shared_ptr<DREG>& Ra, const std::shared_ptr<DREG>& Rx, int dir, bool boundary);

    // Digital Propagation
    void PROP0();

    void PROP1();

    // Vision system functions
    //   Analogue
    void scamp5_get_image(const std::shared_ptr<AREG>& yf, const std::shared_ptr<AREG>& yh, int gain = 1);

    // todo overload all the default ones and add to registration
    void scamp5_in(const std::shared_ptr<AREG>& areg, int16_t value);
    void scamp5_in(const std::shared_ptr<AREG>& areg, int16_t value, std::shared_ptr<AREG>& temp);

    void scamp5_load_in(const std::shared_ptr<AREG>& areg, int8_t value, std::shared_ptr<AREG>& temp);
    void scamp5_load_in(const std::shared_ptr<AREG>& areg, int8_t value);

    void scamp5_load_in(int8_t value);

    void scamp5_load_dac(const std::shared_ptr<AREG>& areg, uint16_t value, std::shared_ptr<AREG>& temp);
    void scamp5_load_dac(const std::shared_ptr<AREG>& areg, uint16_t value);

    void scamp5_load_dac(uint16_t value);

    void scamp5_shift(const std::shared_ptr<AREG>& areg, int h, int v);

    void scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations);
    void scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical);
    void scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical, bool horizontal);
    void scamp5_diffuse(const std::shared_ptr<AREG>& target, int iterations, bool vertical, bool horizontal, std::shared_ptr<AREG>& t0);

    uint8_t scamp5_read_areg(const std::shared_ptr<AREG>& areg, uint8_t r, uint8_t c);

    uint32_t scamp5_global_sum_16(const std::shared_ptr<AREG>& areg, uint8_t* result16v = nullptr);

    uint32_t scamp5_global_sum_64(const std::shared_ptr<AREG>& areg, uint8_t* result64v = nullptr);

    uint8_t scamp5_global_sum_fast(const std::shared_ptr<AREG>& areg);

    uint8_t scamp5_global_sum_sparse(const std::shared_ptr<AREG>& areg, uint8_t r = 2, uint8_t c = 2,
                                     uint8_t rx = 254, uint8_t cx = 254);

    //    Digital
    void scamp5_shift(const std::shared_ptr<DREG>& dreg, int h, int v, int boundary = 0);

    int scamp5_global_or(const std::shared_ptr<DREG>& dreg, uint8_t r = 0, uint8_t c = 0, uint8_t rx = 255, uint8_t cx = 255);

    int scamp5_global_count(const std::shared_ptr<DREG>& dreg, const std::shared_ptr<AREG>& temp, int options = 0);

    void scamp5_flood(const std::shared_ptr<DREG>& dreg_target, const std::shared_ptr<DREG>& dreg_mask, int boundary,
                      int iterations = 5, bool use_local = false);

    void scamp5_load_point(const std::shared_ptr<DREG>& dr, uint8_t r, uint8_t c);

    void scamp5_load_rect(const std::shared_ptr<DREG>& dr, uint8_t r0, uint8_t c0, uint8_t r1,
                          uint8_t c1);

    void scamp5_load_pattern(const std::shared_ptr<DREG>& dr, uint8_t r, uint8_t c, uint8_t rx,
                             uint8_t cx);

    void scamp5_select_point(uint8_t r, uint8_t c);

    void scamp5_select_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1);

    void scamp5_select_pattern(uint8_t r, uint8_t c, uint8_t rx, uint8_t cx);

    void scamp5_select_col(uint8_t c);

    void scamp5_select_row(uint8_t r);

    void scamp5_select_colx(uint8_t cx);

    void scamp5_select_rowx(uint8_t rx);

    void scamp5_draw_begin(const std::shared_ptr<DREG>& dr);

    void scamp5_draw_end();

    void scamp5_draw_pixel(uint8_t r, uint8_t c);

    bool scamp5_draw_point(int r, int c);

    void scamp5_draw_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1);

    void scamp5_draw_line(int r0, int c0, int r1, int c1, bool repeat = false);

    void scamp5_draw_circle(int x0, int y0, int radius, bool repeat = false);

    void scamp5_draw_negate();

    //    Image Readout
    void scamp5_scan_areg(const std::shared_ptr<AREG>& areg, uint8_t* buffer, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs, uint8_t cs);

    void scamp5_scan_areg_8x8(const std::shared_ptr<AREG>& areg, uint8_t* result8x8);

    void scamp5_scan_areg_mean_8x8(const std::shared_ptr<AREG>& areg, uint8_t* result8x8);

    void scamp5_scan_dreg(const std::shared_ptr<DREG>& dreg, uint8_t* mem, uint8_t r0 = 0,
                          uint8_t r1 = 255);

    void scamp5_scan_events(const std::shared_ptr<DREG>& dreg, uint8_t* mem, uint16_t max_num = 1000, uint8_t h_dir = 0, uint8_t v_dir = 0);

    void scamp5_scan_events(const std::shared_ptr<DREG>& dreg, uint8_t* buffer, uint16_t max_num, uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs, uint8_t cs);

    void scamp5_scan_boundingbox(const std::shared_ptr<DREG>& dr, uint8_t* vec4v);

    // Simulator specific methods
    void print_stats();
    RTTR_ENABLE();
};

//class SCAMP5::builder {
//    RTTR_ENABLE();
//
//   private:
//    int rows_ = 256;
//    int cols_ = 256;
//    Origin origin_ = Origin::TOP_LEFT;
//    Config config_;
//    std::unordered_map<std::string, std::shared_ptr<Component>> components_;
//
//   public:
//    builder& with_rows(int rows);
//    builder& with_cols(int cols);
//    builder& with_origin(Origin origin);
//    builder& with_config(Config config);
//    builder& with_components(std::unordered_map<std::string, std::shared_ptr<Component>> components);
//    builder& with_analogue_registers();
//
//    SCAMP5 build();
//};

#endif  // SIMULATOR_SCAMP5_H
