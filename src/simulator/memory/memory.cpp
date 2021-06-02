//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/memory.h"

#include <simulator/memory/dram3t_cell.h>
#include <simulator/memory/si_cell.h>
#include <simulator/memory/sram6t_cell.h>

#include <opencv2/cudaarithm.hpp>

void Memory::init() {
#ifdef TRACK_STATISTICS
    dynamic_read_power_ = calc_dynamic_read();
    dynamic_write_power_ = calc_dynamic_write();
    time_ = this->cycle_count_ * (1.0 / config_->get_clock_rate());
#ifdef USE_CUDA
    scratch = cv::cuda::GpuMat(rows_, cols_, CV_8U, cv::Scalar(0));
#else
    scratch = cv::UMat(rows_, cols_, CV_8U, cv::Scalar(0));
#endif
    Component::init();
#endif
}

#ifdef TRACK_STATISTICS
int Memory::get_cycle_count() {
    return cycle_count_;
}

void Memory::update_static(double time) {
    cv::add(this->array_static_energy_, this->static_power_ * time, this->array_static_energy_, this->internal_mask);
}

void Memory::print_stats(const CycleCounter& counter) {
    std::cerr << "TODO: Implement in memory.cpp" << std::endl;
}

cv::Mat Memory::get_dynamic_energy_array() {
    // add all the accesses with no masks
    cv::add(this->array_dynamic_energy_, read_count_ * this->dynamic_read_power_ * time_, this->array_dynamic_energy_, internal_mask);
    cv::add(this->array_dynamic_energy_, write_count_ * this->dynamic_write_power_ * time_, this->array_dynamic_energy_, internal_mask);

    return Component::get_dynamic_energy_array();
}

void Memory::read(const cv::_InputOutputArray& mask) {
#ifdef USE_CUDA
    cv::cuda::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, mask);
#else
    cv::add(this->array_dynamic_energy_, this->dynamic_read_power_ * time_, this->array_dynamic_energy_, mask);
#endif
}

void Memory::read() {
    read_count_++;
}

void Memory::write(const cv::_InputOutputArray& mask) {
#ifdef USE_CUDA
    cv::cuda::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, mask);
#else
    cv::add(this->array_dynamic_energy_, this->dynamic_write_power_ * time_, this->array_dynamic_energy_, mask);
#endif
}

void Memory::write() {
    write_count_++;
}
#endif

std::shared_ptr<Memory> Memory::construct(MemoryType memory_type, int rows, int cols, int row_stride, int col_stride, const std::shared_ptr<Config>& config) {
    std::shared_ptr<Memory> memory;
    switch (memory_type) {
        case DRAM3T: {
            memory = std::make_shared<Dram3tCell>();
            break;
        }
        case SRAM6T: {
            memory = std::make_shared<Sram6tCell>();
            break;
        }
        case S2I: {
            memory = std::make_shared<SiCell>();
            break;
        };
    }
    memory->set_rows(rows);
    memory->set_cols(cols);
    memory->set_row_stride(row_stride);
    memory->set_col_stride(col_stride);
    memory->set_config(config);
    memory->init();
    return memory;
}
