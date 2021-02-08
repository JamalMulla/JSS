//
// Created by jm1417 on 31/01/2021.
//

#include <opencv2/core.hpp>
#include "register.h"

Register::Register(int rows, int columns, int type, MemoryType memoryType)
    :value_(rows, columns, type),
    read_counter(rows, columns, CV_32S),
    write_counter(rows, columns, CV_32S),
    memory_type_(memoryType){}

void Register::inc_read(const cv::_InputOutputArray& mask) {
    cv::add(this->read_counter, 1, this->read_counter, mask);
}

void Register::inc_write(const cv::_InputOutputArray& mask) {
    cv::add(this->write_counter, 1, this->write_counter, mask);
}

int Register::get_reads() {
    return cv::sum(this->read_counter)[0];
}

int Register::get_writes() {
    return cv::sum(this->write_counter)[0];
}
