//
// Created by jm1417 on 31/01/2021.
//

#include "register.h"

Register::Register(int rows, int columns, int type)
    : value_(rows, columns, type) {}

void Register::inc_read() {
    this->read_counter++;
}

void Register::inc_write() {
    this->write_counter++;
}

int Register::get_reads() {
    return this->read_counter;
}

int Register::get_writes() {
    return this->write_counter;
}
