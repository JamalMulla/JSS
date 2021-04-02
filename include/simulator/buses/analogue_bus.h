//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_BUS_H
#define SIMULATOR_ANALOGUE_BUS_H

#include "simulator/registers/analogue_register.h"
#include "simulator/registers/digital_register.h"

class AnalogueBus {
   public:
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

    void push_north(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                    DigitalRegister& FLAG);
    void push_east(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                   DigitalRegister& FLAG);
    void push_south(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                    DigitalRegister& FLAG);
    void push_west(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                   DigitalRegister& FLAG);

    void pull_north(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                    DigitalRegister& FLAG);
    void pull_east(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                   DigitalRegister& FLAG);
    void pull_south(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                    DigitalRegister& FLAG);
    void pull_west(AnalogueRegister& src, AnalogueRegister& dst, int offset,
                   DigitalRegister& FLAG);

    // Get registers in direction
    void get_east(AnalogueRegister& src, AnalogueRegister& dst, int offset);
    void get_west(AnalogueRegister& src, AnalogueRegister& dst, int offset);
    void get_north(AnalogueRegister& src, AnalogueRegister& dst, int offset);
    void get_south(AnalogueRegister& src, AnalogueRegister& dst, int offset);
};

#endif  // SIMULATOR_ANALOGUE_BUS_H
