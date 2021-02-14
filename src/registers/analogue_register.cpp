//
// Created by jm1417 on 28/01/2021.
//

#include "analogue_register.h"
#include "../memory/si/si.h"
#include "../base/array.h"

AnalogueRegister::AnalogueRegister(int rows, int columns)
    : Register(rows, columns, MAT_TYPE, SI()) { }

Data AnalogueRegister::read() {
    return this->value();
}

void AnalogueRegister::write(Data data) {
    this->value().setTo(data);
}

void AnalogueRegister::write(int data) {
    this->value().setTo(data);
}

AnalogueRegister &AnalogueRegister::operator()(const std::string &name) {
    this->name_= name;
    return *this;
}

void AnalogueRegister::print_stats(const CycleCounter &counter) {

}

void AnalogueRegister::write_stats(const CycleCounter &counter, json& j) {

}
