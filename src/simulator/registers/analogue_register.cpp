//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/registers/analogue_register.h"

#include "simulator/base/array.h"
#include "simulator/memory/si.h"
#include "simulator/metrics/stats.h"

AnalogueRegister::AnalogueRegister(int rows, int columns, Memory &memory) :
    Register(rows, columns, MAT_TYPE, memory) {
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister::AnalogueRegister(int rows, int columns) :
    Register(rows, columns, MAT_TYPE) {
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister::AnalogueRegister(const cv::Mat &data, Memory &memory) :
    Register(data.rows, data.cols, MAT_TYPE, memory) {
    data.copyTo(this->value_);
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister::AnalogueRegister(const cv::Mat &data) :
    Register(data.rows, data.cols, MAT_TYPE) {
    data.copyTo(this->value_);
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister &AnalogueRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

#ifdef TRACK_STATISTICS
//void AnalogueRegister::print_stats(const CycleCounter &counter) {
//    std::cout << counter.to_seconds(stats::CLOCK_RATE) << std::endl;
//}
//
//void AnalogueRegister::write_stats(const CycleCounter &counter, json &j) {
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
