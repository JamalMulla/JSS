//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <opencv2/core/mat.hpp>
#include "simulator/base/component.h"
#include "simulator/memory/memory_type.h"

typedef cv::Mat Data;

class Register : public Component {
protected:
    MemoryType memory_type_;
    Data value_;
    std::string name_;
    cv::Mat read_counter;           // Number of reads
    cv::Mat write_counter;          // Number of writes
    cv::Mat read_energy_counter;    // Energy consumed by reads
    cv::Mat write_energy_counter;   // Energy consumed by writes

public:
    Register(int rows, int columns, int type, MemoryType memoryType);

    Data& value();
    const Data& value() const;

    void change_memory_type(const MemoryType& memory_type);

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

    void print_stats(const CycleCounter& counter) override = 0;
    void write_stats(const CycleCounter& counter, json& j) override = 0;
};


#endif //SIMULATOR_REGISTER_H
