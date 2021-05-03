//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/registers/analogue_register.h"

#include "simulator/base/architecture.h"
#include "simulator/memory/si_cell.h"


AnalogueRegister::AnalogueRegister(int rows, int cols, const std::shared_ptr<Config>& config, int row_stride, int col_stride, MemoryType memory) :
    Register(rows, cols, row_stride, col_stride, MAT_TYPE, config, memory) {
}

AnalogueRegister::AnalogueRegister(int rows, int cols, int row_stride, int col_stride) :
    Register(rows, cols, row_stride, col_stride, MAT_TYPE) {
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister::AnalogueRegister(const cv::Mat &data, int row_stride, int col_stride) :
    Register(data.rows, data.cols, row_stride, col_stride, MAT_TYPE) {
    this->write(data);
    this->min_val = -128;
    this->max_val = 127;
}

AnalogueRegister &AnalogueRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

#ifdef TRACK_STATISTICS
void AnalogueRegister::print_stats(const CycleCounter &counter) {
    std::cout << "===================" << std::endl;
    std::cout << "Name: " << name_ << "\n";
    std::cout << "Static power: " << cv::sum(this->get_static_energy())[0] << std::endl;
    std::cout << "Dynamic power: " << cv::sum(this->get_dynamic_energy())[0] << std::endl;

}
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
