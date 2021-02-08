//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <opencv2/core/mat.hpp>

typedef cv::Mat Data;

class Register {
protected:
    Data value_;
    int read_counter = 0;
    int write_counter = 0;
public:
    Register(int rows, int columns, int type);
    Data & value()       { return value_; }
    [[nodiscard]] const Data & value() const { return value_; }

    void inc_read();
    void inc_write();

    int get_reads();
    int get_writes();

    virtual Data read() = 0;
    virtual void write(Data data) = 0;
    virtual void write(int data) = 0;
};


#endif //SIMULATOR_REGISTER_H
