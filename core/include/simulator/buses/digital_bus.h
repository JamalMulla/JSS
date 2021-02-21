//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_BUS_H
#define SIMULATOR_DIGITAL_BUS_H

#include "simulator/registers/digital_register.h"

class DigitalBus {
public:
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1);
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2);
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3);
    static void NOT(DigitalRegister& d, DigitalRegister& d0);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3);
    static void NOT(DigitalRegister& Rl);
    static void OR(DigitalRegister& Rl, DigitalRegister& Rx);
    static void NOR(DigitalRegister& Rl, DigitalRegister& Rx);
    static void AND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry);
    static void NAND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry);
    static void IMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    static void NIMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    static void XOR(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    static void MOV(DigitalRegister& d, DigitalRegister& d0);
    static void MUX(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& Rz);
    static void CLR_IF(DigitalRegister& Rl, DigitalRegister& Rx);

    // Digital Logic Operations
    static void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& FLAG);
    static void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& FLAG);
    static void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3, DigitalRegister& FLAG);
    static void NOT_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& FLAG);
    static void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& FLAG);
    static void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& FLAG);
    static void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3, DigitalRegister& FLAG);
    static void NOT_MASKED(DigitalRegister& Rl, DigitalRegister& FLAG);
    static void OR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);
    static void NOR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);
    static void AND_MASKED(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void NAND_MASKED(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void IMP_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void NIMP_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void XOR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void MOV_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& FLAG);
    static void MUX_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& Rz, DigitalRegister& FLAG);
    static void CLR_IF_MASKED(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);

};


#endif //SIMULATOR_DIGITAL_BUS_H
