//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_BUS_H
#define SIMULATOR_ANALOGUE_BUS_H


#include "../registers/analogue_register.h"
#include "../registers/digital_register.h"

class AnalogueBus {

public:
    // Analogue Register Transfer
    void bus(AnalogueRegister& a, const DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, const AnalogueRegister& a0, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, const AnalogueRegister& a0, const AnalogueRegister& a1, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, const AnalogueRegister& a0, const AnalogueRegister& a1, const AnalogueRegister& a2, DigitalRegister& FLAG);
    void bus(AnalogueRegister& a, const AnalogueRegister& a0, const AnalogueRegister& a1, const AnalogueRegister& a2, const AnalogueRegister& a3, DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, const AnalogueRegister& a0, DigitalRegister& FLAG);
    void bus2(AnalogueRegister& a, AnalogueRegister& b, const AnalogueRegister& a0, const AnalogueRegister& a1, DigitalRegister& FLAG);
    void bus3(AnalogueRegister& a, AnalogueRegister& b, AnalogueRegister& c, const AnalogueRegister& a0, DigitalRegister& FLAG);
    void conditional_positive_set(const AnalogueRegister& a, DigitalRegister& b);
    void conditional_positive_set(const AnalogueRegister& a0, const AnalogueRegister& a1, DigitalRegister& b);
    void conditional_positive_set(const AnalogueRegister& a0, const AnalogueRegister& a1, const AnalogueRegister& a2, DigitalRegister& b);
    void mov(AnalogueRegister& y, const AnalogueRegister& x0, AnalogueRegister& intermediate, DigitalRegister& FLAG);

    // Analogue Arithmetic
    void res(AnalogueRegister& a);
    void res(AnalogueRegister& a, AnalogueRegister& b);

    void add(AnalogueRegister& y, const AnalogueRegister& x0, const AnalogueRegister& x1, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void add(AnalogueRegister& y, const AnalogueRegister& x0, const AnalogueRegister& x1, const AnalogueRegister& x2, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void sub(AnalogueRegister& y, const AnalogueRegister& x0, const AnalogueRegister& x1, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void neg(AnalogueRegister& y, const AnalogueRegister& x0, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void abs(AnalogueRegister& y, const AnalogueRegister& x0, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void div(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void div(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2, const AnalogueRegister& x0,AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void diva(AnalogueRegister& y0, AnalogueRegister& y1, AnalogueRegister& y2, AnalogueRegister& intermediate, DigitalRegister& FLAG);
    void divq(AnalogueRegister& y0, const AnalogueRegister& x0, AnalogueRegister& intermediate, DigitalRegister& FLAG);

    // Analogue Neighbour Access

    void move_neighbour(AnalogueRegister& a, AnalogueRegister& b, int offset_x, int offset_y, DigitalRegister& FLAG);

//    void push_north(const AnalogueRegister& a, AnalogueRegister& b, int offset, DigitalRegister& FLAG);
//    void push_east(const AnalogueRegister& a, AnalogueRegister& b, int offset, DigitalRegister& FLAG);
//    void push_south(const AnalogueRegister& a, AnalogueRegister& b, int offset, DigitalRegister& FLAG);
//    void push_west(const AnalogueRegister& a, AnalogueRegister& b, int offset, DigitalRegister& FLAG);

    // Get registers in direction
    void get_east(const AnalogueRegister &src, AnalogueRegister& dst, int offset);
    void get_west(const AnalogueRegister &src, AnalogueRegister& dst, int offset);
    void get_north(const AnalogueRegister &src, AnalogueRegister& dst, int offset);
    void get_south(const AnalogueRegister &src, AnalogueRegister& dst, int offset);

    void push_north(AnalogueRegister& reg, int offset, DigitalRegister& FLAG);
    void push_east(AnalogueRegister& reg, int offset, DigitalRegister& FLAG);
    void push_south(AnalogueRegister& reg, int offset, DigitalRegister& FLAG);
    void push_west(AnalogueRegister& reg, int offset, DigitalRegister& FLAG);


};


#endif //SIMULATOR_ANALOGUE_BUS_H
