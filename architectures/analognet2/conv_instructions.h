/*
 * conv_instructions.hpp
 *
 *  Created on: Jul 4, 2019
 *      Author: Benoît GUILLARD
 *
 *  Modified on: Mar 4, 2021
 *      Author: Jamal Mulla
 */
#ifndef CONV_INSTRUCTIONS_H_
#define CONV_INSTRUCTIONS_H_

#include "../scamp5.h"

//Convolution: Filter A
inline void conv_A(SCAMP5& s){
    s.movx(s.A, s.A, west);
    s.diva(s.A, s.E, s.F);
    s.diva(s.A, s.E, s.F);
    s.movx(s.D, s.A, south);
    s.movx(s.D, s.D, east);
    s.movx(s.E, s.A, north);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.sub(s.A, s.A, s.E);
    s.movx(s.E, s.D, west);
    s.sub(s.A, s.A, s.E);
    s.movx(s.E, s.D, north);
    s.movx(s.E, s.E, north);
    s.sub(s.D, s.D, s.E);
    s.add(s.A, s.A, s.D);
    s.movx(s.D, s.D, east);

    s.mov(s.F, s.D);
    s.add(s.D, s.D, s.F);

    s.add(s.A, s.A, s.D);
}

//Convolution: Filter B
inline void conv_B(SCAMP5& s){
    s.movx(s.B, s.B, east);
    s.diva(s.B, s.E, s.F);
    s.diva(s.B, s.E, s.F);
    s.movx(s.D, s.B, west);

    s.neg(s.F, s.D);
    s.mov(s.D, s.F);

    s.movx(s.E, s.B, north);
    s.add(s.B, s.B, s.E);
    s.add(s.B, s.B, s.D);
    s.movx(s.D, s.D, east);
    s.movx(s.E, s.B, south);
    s.add(s.B, s.B, s.E);
    s.add(s.B, s.B, s.D);
    s.movx(s.D, s.D, north);
    s.movx(s.D, s.D, west);

    s.mov(s.F, s.D);
    s.add(s.D, s.D, s.F);

    s.add(s.B, s.B, s.D);
}

//Convolution: Filter C
inline void conv_C(SCAMP5& s){
    s.movx(s.C, s.C, west);
    s.diva(s.C, s.E, s.F);
    s.diva(s.C, s.E, s.F);
    s.movx(s.D, s.C, east);
    s.movx(s.D, s.D, east);

    s.neg(s.F, s.D);
    s.mov(s.D, s.F);

    s.movx(s.E, s.C, south);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.add(s.C, s.C, s.E);
    s.movx(s.E, s.D, south);
    s.movx(s.E, s.E, west);
    s.movx(s.E, s.E, west);
    s.sub(s.C, s.C, s.E);
    s.movx(s.E, s.D, north);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.sub(s.C, s.C, s.E);

    s.movx(s.E, s.D, south);
    s.movx(s.E, s.E, west);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.add(s.C, s.C, s.E);
    s.movx(s.E, s.D, south);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.mov(s.F, s.E);
    s.add(s.E, s.E, s.F);

    s.add(s.C, s.C, s.E);
    s.movx(s.E, s.D, north);
    s.add(s.C, s.C, s.E);
    s.movx(s.E, s.D, north);
    s.movx(s.E, s.E, west);
    s.add(s.D, s.D, s.E);
    s.add(s.C, s.C, s.D);
    s.movx(s.D, s.D, west);

    s.mov(s.F, s.D);
    s.add(s.D, s.D, s.F);

    s.mov(s.F, s.D);
    s.add(s.D, s.D, s.F);

    s.add(s.C, s.C, s.D);
}

#endif /* CONV_INSTRUCTIONS_H_ */
