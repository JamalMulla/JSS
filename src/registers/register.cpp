//
// Created by jm1417 on 31/01/2021.
//

#include <opencv2/core.hpp>
#include "register.h"
#include "../stats.h"

Register::Register(int rows, int columns, int type, MemoryType memoryType)
    :value_(rows, columns, type),
    read_counter(rows, columns, CV_32S),
    write_counter(rows, columns, CV_32S),
    read_energy_counter(rows, columns, CV_32F),
    write_energy_counter(rows, columns, CV_32F),
    memory_type_(memoryType){}

void Register::inc_read(const cv::_InputOutputArray& mask) {
    cv::add(this->read_counter, 1, this->read_counter, mask);
    int number_of_cycle = 2; // TODO Needs to come from somewhere
    double energy_usage = this->memory_type_.read_power_draw * stats::CYCLE_TIME * number_of_cycle;
    cv::add(this->read_energy_counter, energy_usage, this->read_energy_counter, mask);
}

void Register::inc_write(const cv::_InputOutputArray& mask) {
    cv::add(this->write_counter, 1, this->write_counter, mask);
    int number_of_cycle = 1; // TODO Needs to come from somewhere
    double energy_usage = this->memory_type_.write_power_draw * stats::CYCLE_TIME * number_of_cycle;
    cv::add(this->write_energy_counter, energy_usage, this->write_energy_counter, mask);
}

int Register::get_reads() {
    return cv::sum(this->read_counter)[0];
}

int Register::get_writes() {
    return cv::sum(this->write_counter)[0];
}

double Register::get_read_energy() {
    return cv::sum(this->read_energy_counter)[0];
}

double Register::get_write_energy() {
    return cv::sum(this->write_energy_counter)[0];
}

double Register::get_total_energy() {
    return get_read_energy() + get_write_energy();
}

