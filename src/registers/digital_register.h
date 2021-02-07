//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "register.h"

class DigitalRegister : public Register {
public:
    DigitalRegister(int rows, int columns);
    void set();
    void clear();

    Data read() override;
    void write(Data data) override;
    void write(int data) override;
};


#endif //SIMULATOR_DIGITAL_REGISTER_H
