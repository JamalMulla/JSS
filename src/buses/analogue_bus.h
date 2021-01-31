//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ANALOGUE_BUS_H
#define SIMULATOR_ANALOGUE_BUS_H

#include "../../architectures/scamp5.h"

class AnalogueBus {
public:
    // Analog Register Transfer
    static void bus(AREG& a, const DREG& FLAG);
    static void bus(AREG& a, const AREG& a0, DREG& FLAG);
    static void bus(AREG& a, const AREG& a0, const AREG& a1, DREG& FLAG);
    static void bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2, DREG& FLAG);
    static void bus(AREG& a, const AREG& a0, const AREG& a1, const AREG& a2, const AREG& a3, DREG& FLAG);
    static void bus2(AREG& a, AREG& b, DREG& FLAG);
    static void bus2(AREG& a, AREG& b, const AREG& a0, DREG& FLAG);
    static void bus2(AREG& a, AREG& b, const AREG& a0, const AREG& a1, DREG& FLAG);
    static void bus3(AREG& a, AREG& b, AREG& c, const AREG& a0, DREG& FLAG);
    static void conditional_positive_set(const AREG& a, DREG& b);
    static void conditional_positive_set(const AREG& a0, const AREG& a1, DREG& b);
    static void conditional_positive_set(const AREG& a0, const AREG& a1, const AREG& a2, DREG& b);
    static void mov(AREG& y, const AREG& x0, AREG& intermediate, DREG& FLAG);

    // Analog Arithmetic
    static void res(AREG& a);
    static void res(AREG& a, AREG& b);

    static void add(AREG& y, const AREG& x0, const AREG& x1, AREG& intermediate, DREG& FLAG);
    static void add(AREG& y, const AREG& x0, const AREG& x1, const AREG& x2, AREG& intermediate, DREG& FLAG);
    static void sub(AREG& y, const AREG& x0, const AREG& x1, AREG& intermediate, DREG& FLAG);
    static void neg(AREG& y, const AREG& x0, AREG& intermediate, DREG& FLAG);
    static void abs(AREG& y, const AREG& x0, AREG& intermediate, DREG& FLAG);
    static void div(AREG& y0, AREG& y1, AREG& y2, AREG& intermediate, DREG& FLAG);
    static void div(AREG& y0, AREG& y1, AREG& y2, const AREG& x0,AREG& intermediate, DREG& FLAG);
    static void diva(AREG& y0, AREG& y1, AREG& y2, AREG& intermediate, DREG& FLAG);
    static void divq(AREG& y0, const AREG& x0, AREG& intermediate, DREG& FLAG);

};


#endif //SIMULATOR_ANALOGUE_BUS_H
