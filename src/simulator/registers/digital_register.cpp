//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/registers/digital_register.h"

#include <iostream>

#include "simulator/metrics/stats.h"

DigitalRegister::DigitalRegister(int rows, int columns, Memory &memory_type) :
    Register(rows, columns, CV_8U, memory_type) {
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister::DigitalRegister(int rows, int columns) :
    Register(rows, columns, CV_8U) {
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister::DigitalRegister(const cv::Mat &data, Memory &memory_type) :
    Register(data.rows, data.cols, CV_8U, memory_type) {
    data.copyTo(this->value_);
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister::DigitalRegister(const cv::Mat &data) :
    Register(data.rows, data.cols, CV_8U) {
    data.copyTo(this->value_);
    this->min_val = 0;
    this->max_val = 1;
}

DigitalRegister &DigitalRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

void DigitalRegister::set() { this->write(1); }

void DigitalRegister::clear() { this->write(0); }

#ifdef TRACK_STATISTICS
//void DigitalRegister::print_stats(const CycleCounter &counter) {
//    // power x time = energy
//    // energy / time = power
//
//    // Keep track of energy usage.
//    // So each write do + write_access_time (in) * write_power_draw
//    // and same for read
//
//    std::cout << "Register: " << this->name_ << std::endl;
//
//    std::cout << "Energy consumed by reads: " << this->get_read_energy()
//              << " joules" << std::endl;
//    std::cout << "Energy consumed by writes: " << this->get_write_energy()
//              << " joules" << std::endl;
//    std::cout << "Total energy: " << this->get_total_energy() << " joules"
//              << std::endl;
//
//    // convert number of cycles to seconds based off clock rate
//    double runtime_in_seconds = counter.to_seconds(stats::CLOCK_RATE);
//
//    std::cout << "Average power for reads: "
//              << this->get_read_energy() / runtime_in_seconds << " watts"
//              << std::endl;
//    std::cout << "Average power for writes: "
//              << this->get_write_energy() / runtime_in_seconds << " watts"
//              << std::endl;
//    std::cout << "Total average power: "
//              << this->get_total_energy() / runtime_in_seconds << " watts"
//              << std::endl;
//}
//
//void DigitalRegister::write_stats(const CycleCounter &counter, json &j) {
//    double runtime = counter.to_seconds(stats::CLOCK_RATE);
//    auto reg_stats = json::object();
//    reg_stats[this->name_] = {
//        {"Energy (J)",
//         {
//             {"Reads", this->get_read_energy()},
//             {"Writes", this->get_write_energy()},
//             {"Total", this->get_total_energy()},
//         }},
//        {"Power (W)",
//         {
//             {"Reads", this->get_read_energy() / runtime},
//             {"Writes", this->get_write_energy() / runtime},
//             {"Total", this->get_total_energy() / runtime},
//         }},
//        {"Accesses",
//         {{"Reads", this->get_reads()}, {"Writes", this->get_writes()}}
//
//        }};
//    j.push_back(reg_stats);
//}
#endif
