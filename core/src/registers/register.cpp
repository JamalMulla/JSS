//
// Created by jm1417 on 31/01/2021.
//

#include <opencv2/core.hpp>
#include "simulator/registers/register.h"
#include "simulator/metrics/stats.h"

Register::Register(int rows, int columns, int type, MemoryType memoryType)
    :value_(rows, columns, type, cv::Scalar(0)),
    read_counter(rows, columns, CV_32S, cv::Scalar(0)),
    write_counter(rows, columns, CV_32S, cv::Scalar(0)),
    read_energy_counter(rows, columns, CV_64F, cv::Scalar(0)),
    write_energy_counter(rows, columns, CV_64F, cv::Scalar(0)),
    reads(0),
    writes(0),
    memory_type_(memoryType){}

void Register::inc_read(const cv::_InputOutputArray& mask) {
    cv::add(this->read_counter, 1, this->read_counter, mask);
    int number_of_cycle = 1;  // Should be parameterisable
    double energy_usage = this->memory_type_.read_power_draw * stats::CYCLE_TIME * number_of_cycle;
    cv::add(this->read_energy_counter, energy_usage, this->read_energy_counter, mask);
    this->reads++;
}

void Register::inc_write(const cv::_InputOutputArray& mask) {
    cv::add(this->write_counter, 1, this->write_counter, mask);
    int number_of_cycle = 1;
    double energy_usage = this->memory_type_.write_power_draw * stats::CYCLE_TIME * number_of_cycle;
    cv::add(this->write_energy_counter, energy_usage, this->write_energy_counter, mask);
    this->writes++;
}

int Register::get_reads() {
    return this->reads;
}

int Register::get_writes() {
    return this->writes;
}

int Register::get_total_reads() {
    return cv::sum(this->read_counter)[0];
}

int Register::get_total_writes() {
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

Data &Register::value() {
    // For reading
    return this->value_;
}

const Data &Register::value() const {
    // For writing
    return this->value_;
}

void Register::change_memory_type(const MemoryType &memory_type) {
    this->memory_type_ = memory_type;
}


