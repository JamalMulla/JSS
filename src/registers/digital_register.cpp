//
// Created by jm1417 on 28/01/2021.
//

#include "digital_register.h"

void DigitalRegister::set() {
    this->value().setTo(1);
}

void DigitalRegister::set(const DigitalRegister& FLAG) {
    this->value().setTo(1, FLAG.value());
}

void DigitalRegister::clear() {
    this->value().setTo(0);
}

void DigitalRegister::clear(const DigitalRegister& FLAG) {
    this->value().setTo(0, FLAG.value());
}
