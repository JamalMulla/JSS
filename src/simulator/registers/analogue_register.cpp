//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/registers/analogue_register.h"

#include "simulator/base/architecture.h"
#include "simulator/memory/si_cell.h"

AnalogueRegister::AnalogueRegister(int rows, int cols, const std::shared_ptr<Config>& config, int row_stride, int col_stride, MemoryType memory) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->config_ = config;
    this->type_ = MAT_TYPE;
    this->set_memory(memory);
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
}

AnalogueRegister::AnalogueRegister(int rows, int cols, int row_stride, int col_stride) {
    this->rows_ = rows;
    this->cols_ = cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->type_ = MAT_TYPE;
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
}

AnalogueRegister::AnalogueRegister(const cv::Mat &data, int row_stride, int col_stride) {
    this->rows_ = data.rows;
    this->cols_ = data.cols;
    this->row_stride_ = row_stride;
    this->col_stride_ = col_stride;
    this->type_ = MAT_TYPE;
    Register::init();
    this->min_val = 0;
    this->max_val = 1;
    this->write(data);
}

AnalogueRegister &AnalogueRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

#ifdef TRACK_STATISTICS
void AnalogueRegister::print_stats(const CycleCounter &counter) {
    std::cout << "===================" << std::endl;
    std::cout << "Name: " << name_ << "\n";
    std::cout << "Static power: " << cv::sum(this->get_static_energy_array())[0] << std::endl;
    std::cout << "Dynamic power: " << cv::sum(this->get_dynamic_energy_array())[0] << std::endl;

}
#endif
