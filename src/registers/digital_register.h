//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "register.h"

class DigitalRegister : public Register {
public:
    void set();
    void set(const DigitalRegister& FLAG);
    void clear();
    void clear(const DigitalRegister& FLAG);
};


#endif //SIMULATOR_DIGITAL_REGISTER_H
