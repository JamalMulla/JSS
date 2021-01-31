//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "register.h"
#include "../../architectures/scamp5.h"

class DigitalRegister : public Register {
public:
    void set();
    void clear();
};


#endif //SIMULATOR_DIGITAL_REGISTER_H
