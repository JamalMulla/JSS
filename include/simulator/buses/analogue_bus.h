//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_BUS_H
#define SIMULATOR_ANALOGUE_BUS_H

#include "simulator/base/plane_params.h"

#include "simulator/registers/analogue_register.h"
#include "simulator/registers/digital_register.h"

class AnalogueBus {
   private:
    cv::Mat scratch;

   public:
    // Hard sigmoid
    void hard_sigmoid(AnalogueRegister& a, AnalogueRegister& a0, DigitalRegister& FLAG);

    // Analogue Register Transfer
    void bus(AnalogueRegister& a, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, AnalogueRegister& a0, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, AnalogueRegister& a0, AnalogueRegister& a1,
             DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, AnalogueRegister& a0, AnalogueRegister& a1,
             AnalogueRegister& a2, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, AnalogueRegister& a0, AnalogueRegister& a1,
             AnalogueRegister& a2, AnalogueRegister& a3, DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, AnalogueRegister& a0,
              DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, AnalogueRegister& a0,
              AnalogueRegister& a1, DigitalRegister& FLAG);
    void bus3(AnalogueRegister& a, AnalogueRegister& b, AnalogueRegister& c,
              AnalogueRegister& a0, DigitalRegister& FLAG);
    void conditional_positive_set(DigitalRegister& b, AnalogueRegister& a);
    void conditional_positive_set(DigitalRegister& b, AnalogueRegister& a0,
                                  AnalogueRegister& a1);
    void conditional_positive_set(DigitalRegister& b, AnalogueRegister& a0,
                                  AnalogueRegister& a1, AnalogueRegister& a2);
    void mov(AnalogueRegister& y, AnalogueRegister& x0,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);

    // Analogue Arithmetic
    void res(AnalogueRegister& a);
    void res(AnalogueRegister& a, AnalogueRegister& b);

    void add(AnalogueRegister& y, AnalogueRegister& x0, AnalogueRegister& x1,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void add(AnalogueRegister& y, AnalogueRegister& x0, AnalogueRegister& x1,
             AnalogueRegister& x2, AnalogueRegister& intermediate,
             DigitalRegister& FLAG);
    void sub(AnalogueRegister& y, AnalogueRegister& x0, AnalogueRegister& x1,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void neg(AnalogueRegister& y, AnalogueRegister& x0,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void abs(AnalogueRegister& y, AnalogueRegister& x0,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void div(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2,
             AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void div(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2,
             AnalogueRegister& x0, AnalogueRegister& intermediate,
             DigitalRegister& FLAG);
    void diva(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2,
              AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void divq(AnalogueRegister& y0, AnalogueRegister& x0,
              AnalogueRegister& intermediate, DigitalRegister& FLAG);

    // Neighbour Access

    // Get registers in direction
    /* These first neighbour functions are absolute and do not depend on origin.
     * They function as expected assuming a grid with origin in bottom left */
    void get_up(AnalogueRegister& dst, AnalogueRegister& src, int offset);
    void get_right(AnalogueRegister& dst, AnalogueRegister& src, int offset);
    void get_left(AnalogueRegister& dst, AnalogueRegister& src, int offset);
    void get_down(AnalogueRegister& dst, AnalogueRegister& src, int offset);

    /* The following are dependent on origin. For example, North when the origin
     * is at the bottom is not the same as North when the origin is at the top.
     * Top north = bottom south*/
    void get_east(AnalogueRegister& dst, AnalogueRegister& src, int offset, Origin origin);
    void get_west(AnalogueRegister& dst, AnalogueRegister& src, int offset, Origin origin);
    void get_north(AnalogueRegister& dst, AnalogueRegister& src, int offset,Origin origin);
    void get_south(AnalogueRegister& dst, AnalogueRegister& src, int offset, Origin origin);

    // Higher level functions
    void scan(uint8_t* dst, AnalogueRegister& src, uint8_t row_start,
              uint8_t col_start, uint8_t row_end, uint8_t col_end,
              uint8_t row_step, uint8_t col_step, Origin origin);
    void blocked_average(uint8_t* result, AnalogueRegister& src, int block_size,
                         Origin origin);
};

#endif  // SIMULATOR_ANALOGUE_BUS_H
