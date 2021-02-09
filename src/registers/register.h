//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "../base/component.h"
#include "../memory/memory_type.h"

typedef cv::Mat Data;

class Register : public Component {
protected:
    MemoryType memory_type_;
    Data value_;
    cv::Mat read_counter;           // Number of reads
    cv::Mat write_counter;          // Number of writes
    cv::Mat read_energy_counter;    // Energy consumed by reads
    cv::Mat write_energy_counter;   // Energy consumed by writes

public:
    Register(int rows, int columns, int type, MemoryType memoryType);
    Data & value()       { return value_; }
    [[nodiscard]] const Data & value() const { return value_; }

    void inc_read(const cv::_InputOutputArray& mask = cv::noArray());
    void inc_write(const cv::_InputOutputArray& mask = cv::noArray());

    int get_reads();
    int get_writes();
    double get_read_energy();
    double get_write_energy();
    double get_total_energy();

    virtual Data read() = 0;
    virtual void write(Data data) = 0;
    virtual void write(int data) = 0;

    void print_stats() override = 0;
};


#endif //SIMULATOR_REGISTER_H
