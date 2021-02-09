//
// Created by jm1417 on 28/01/2021.
//

#include "analogue_register.h"
#include "../memory/si/si.h"

AnalogueRegister::AnalogueRegister(int rows, int columns)
    : Register(rows, columns, CV_16S, SI()) { }

Data AnalogueRegister::read() {
    return this->value();
}

void AnalogueRegister::write(Data data) {
    this->value().setTo(data);
}

void AnalogueRegister::write(int data) {
    this->value().setTo(data);
}

void AnalogueRegister::print_stats() {

}
