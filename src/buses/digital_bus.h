//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_BUS_H
#define SIMULATOR_DIGITAL_BUS_H

#include "../registers/digital_register.h"

class DigitalBus {
public:
    // Digital Logic Operations
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& FLAG);
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& FLAG);
    static void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3, DigitalRegister& FLAG);
    static void NOT(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& FLAG);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& FLAG);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& FLAG);
    static void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1, DigitalRegister& d2, DigitalRegister& d3, DigitalRegister& FLAG);
    static void NOT(DigitalRegister& Rl, DigitalRegister& FLAG);
    static void OR(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);
    static void NOR(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);
    static void AND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void NAND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void IMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void NIMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void XOR(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& FLAG);
    static void MOV(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& FLAG);
    static void MUX(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry, DigitalRegister& Rz, DigitalRegister& FLAG);
    static void CLR_IF(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& FLAG);
};


#endif //SIMULATOR_DIGITAL_BUS_H
