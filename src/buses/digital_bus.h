//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_BUS_H
#define SIMULATOR_DIGITAL_BUS_H

#include "../../architectures/scamp5.h"

class DigitalBus {
public:
    // Digital Logic Operations
    static void OR(DREG& d, DREG& d0, DREG& d1, DREG& FLAG);
    static void OR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& FLAG);
    static void OR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3, DREG& FLAG);
    static void NOT(DREG& d, DREG& d0, DREG& FLAG);
    static void NOR(DREG& d, DREG& d0, DREG& d1, DREG& FLAG);
    static void NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& FLAG);
    static void NOR(DREG& d, DREG& d0, DREG& d1, DREG& d2, DREG& d3, DREG& FLAG);
    static void NOT(DREG& Rl, DREG& FLAG);
    static void OR(DREG& Rl, DREG& Rx, DREG& FLAG);
    static void NOR(DREG& Rl, DREG& Rx, DREG& FLAG);
    static void AND(DREG& Ra, DREG& Rx, DREG& Ry, DREG& FLAG);
    static void NAND(DREG& Ra, DREG& Rx, DREG& Ry, DREG& FLAG);
    static void IMP(DREG& Rl, DREG& Rx, DREG& Ry, DREG& FLAG);
    static void NIMP(DREG& Rl, DREG& Rx, DREG& Ry, DREG& FLAG);
    static void XOR(DREG& Rl, DREG& Rx, DREG& Ry, DREG& FLAG);

    static void MOV(DREG& d, DREG& d0, DREG& FLAG);
    static void MUX(DREG& Rl, DREG& Rx, DREG& Ry, DREG& Rz, DREG& FLAG);
    static void CLR_IF(DREG& Rl, DREG& Rx, DREG& FLAG);
};


#endif //SIMULATOR_DIGITAL_BUS_H
