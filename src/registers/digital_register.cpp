//
// Created by jm1417 on 28/01/2021.
//

#include <iostream>
#include "digital_register.h"

DigitalRegister::DigitalRegister(int rows, int columns, const MemoryType& memory_type)
    : Register(rows, columns, CV_8U, memory_type) { }

Data DigitalRegister::read() {
    this->inc_read();
    return this->value();
}

void DigitalRegister::write(Data data) {
    this->value().setTo(data);
    this->inc_write();
}

void DigitalRegister::write(int data) {
    this->value().setTo(data);
    this->inc_write();
}

void DigitalRegister::set() {
    this->write(1);
}

void DigitalRegister::clear() {
    this->write(0);
}

void DigitalRegister::print_stats() {
    // power x time = energy
    // energy / time = power

    // Keep track of energy usage.
    // So each write do + write_access_time (in) * write_power_draw
    // and same for read

    std::cout << "Energy consumed by reads: " << this->get_read_energy() << " joules?" << std::endl;
    std::cout << "Energy consumed by writes: " << this->get_write_energy() << " joules?" << std::endl;
    std::cout << "Total energy: " << this->get_total_energy() << " joules?" << std::endl;
}



