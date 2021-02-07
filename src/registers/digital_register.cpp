//
// Created by jm1417 on 28/01/2021.
//

#include "digital_register.h"

DigitalRegister::DigitalRegister(int rows, int columns)
    : Register(rows, columns, CV_8U) { }

Data DigitalRegister::read() {
    return this->value();
}

void DigitalRegister::write(Data data) {
    this->value().setTo(data);
}

void DigitalRegister::write(int data) {
    this->value().setTo(data);
}

void DigitalRegister::set() {
    this->write(1);
}

void DigitalRegister::clear() {
    this->write(0);
}



