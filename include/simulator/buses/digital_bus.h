//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_BUS_H
#define SIMULATOR_DIGITAL_BUS_H

#include "simulator/base/plane_params.h"
#include "simulator/registers/analogue_register.h"
#include "simulator/registers/digital_register.h"

class DigitalBus {
   public:
    void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1);
    void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
            DigitalRegister& d2);
    void OR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
            DigitalRegister& d2, DigitalRegister& d3);
    void NOT(DigitalRegister& d, DigitalRegister& d0);
    void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1);
    void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
             DigitalRegister& d2);
    void NOR(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
             DigitalRegister& d2, DigitalRegister& d3);
    void NOT(DigitalRegister& Rl);
    void OR(DigitalRegister& Rl, DigitalRegister& Rx);
    void NOR(DigitalRegister& Rl, DigitalRegister& Rx);
    void AND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry);
    void NAND(DigitalRegister& Ra, DigitalRegister& Rx, DigitalRegister& Ry);
    void IMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    void NIMP(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    void XOR(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry);
    void MOV(DigitalRegister& d, DigitalRegister& d0);
    void MUX(DigitalRegister& Rl, DigitalRegister& Rx, DigitalRegister& Ry,
             DigitalRegister& Rz);
    void CLR_IF(DigitalRegister& Rl, DigitalRegister& Rx);

    // Digital Logic Operations
    void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
                   DigitalRegister& FLAG);
    void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
                   DigitalRegister& d2, DigitalRegister& FLAG);
    void OR_MASKED(DigitalRegister& d, DigitalRegister& d0, DigitalRegister& d1,
                   DigitalRegister& d2, DigitalRegister& d3,
                   DigitalRegister& FLAG);
    void NOT_MASKED(DigitalRegister& d, DigitalRegister& d0,
                    DigitalRegister& FLAG);
    void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0,
                    DigitalRegister& d1, DigitalRegister& FLAG);
    void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0,
                    DigitalRegister& d1, DigitalRegister& d2,
                    DigitalRegister& FLAG);
    void NOR_MASKED(DigitalRegister& d, DigitalRegister& d0,
                    DigitalRegister& d1, DigitalRegister& d2,
                    DigitalRegister& d3, DigitalRegister& FLAG);
    void NOT_MASKED(DigitalRegister& Rl, DigitalRegister& FLAG);
    void OR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                   DigitalRegister& FLAG);
    void NOR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                    DigitalRegister& FLAG);
    void AND_MASKED(DigitalRegister& Ra, DigitalRegister& Rx,
                    DigitalRegister& Ry, DigitalRegister& FLAG);
    void NAND_MASKED(DigitalRegister& Ra, DigitalRegister& Rx,
                     DigitalRegister& Ry, DigitalRegister& FLAG);
    void IMP_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                    DigitalRegister& Ry, DigitalRegister& FLAG);
    void NIMP_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                     DigitalRegister& Ry, DigitalRegister& FLAG);
    void XOR_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                    DigitalRegister& Ry, DigitalRegister& FLAG);
    void MOV_MASKED(DigitalRegister& d, DigitalRegister& d0,
                    DigitalRegister& FLAG);
    void MUX_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                    DigitalRegister& Ry, DigitalRegister& Rz,
                    DigitalRegister& FLAG);
    void CLR_IF_MASKED(DigitalRegister& Rl, DigitalRegister& Rx,
                       DigitalRegister& FLAG);

    // Neighbour Operations

    /* These first neighbour functions are absolute and do not depend on origin.
     * They function as expected assuming a grid with origin in bottom left */
    void get_up(DigitalRegister& dst, DigitalRegister& src, int offset,
                int boundary_fill);
    void get_right(DigitalRegister& dst, DigitalRegister& src, int offset,
                   int boundary_fill);
    void get_left(DigitalRegister& dst, DigitalRegister& src, int offset,
                  int boundary_fill);
    void get_down(DigitalRegister& dst, DigitalRegister& src, int offset,
                  int boundary_fill);

    /* The following are dependent on origin. For example, North when the origin
     * is at the bottom is not the same as North when the origin is at the top.
     * Top north = bottom south*/
    void get_east(DigitalRegister& dst, DigitalRegister& src, int offset,
                  int boundary_fill, Origin origin);
    void get_west(DigitalRegister& dst, DigitalRegister& src, int offset,
                  int boundary_fill, Origin origin);
    void get_north(DigitalRegister& dst, DigitalRegister& src, int offset,
                   int boundary_fill, Origin origin);
    void get_south(DigitalRegister& dst, DigitalRegister& src, int offset,
                   int boundary_fill, Origin origin);

    // Higher level functions

    // Superpixel Operations
    void superpixel_create(DigitalRegister& dst, AnalogueRegister& src,
                           const std::unordered_map<int, cv::Point>& locations);
    void superpixel_shift_right(DigitalRegister& dst,
        DigitalRegister& src,
        Origin origin);
    void positions_from_bitorder(
        std::vector<std::vector<std::vector<int>>> bitorder, int banks,
        int height, int width, std::unordered_map<int, cv::Point>& locations);
};

#endif  // SIMULATOR_DIGITAL_BUS_H
