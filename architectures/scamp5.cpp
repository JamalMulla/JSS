//
// Created by jm1417 on 31/01/2021.
//

#include "scamp5.h"

void SCAMP5::nop() {

}

void SCAMP5::rpix() {
    //reset PIX
    this->pe.photodiode.reset();
}

void SCAMP5::get_image(AREG &y) {
    //y := half-range image, and reset PIX
    this->pe.bus.
}
