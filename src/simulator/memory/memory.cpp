//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/memory.h"

#include <simulator/memory/dram3t_cell.h>
#include <simulator/memory/si_cell.h>
#include <simulator/memory/sram6t_cell.h>

void Memory::init() {
    internal_mask = cv::Mat(rows_, cols_, CV_8U, cv::Scalar(0));
#ifdef TRACK_STATISTICS
    array_transistor_count_ = cv::Mat(rows_, cols_, CV_32S, cv::Scalar(0));
    array_static_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
    array_dynamic_energy_ = cv::Mat(rows_, cols_, CV_64F, cv::Scalar(0));
#endif
    this->calc_internal_mask();
}

std::shared_ptr<Memory> Memory::construct(MemoryType memory_type, int rows, int cols, int row_stride, int col_stride, const std::shared_ptr<Config>& config) {
    std::shared_ptr<Memory> memory;
    switch(memory_type) {
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

