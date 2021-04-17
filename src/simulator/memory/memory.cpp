//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/memory.h"

#include <simulator/memory/dram3t_cell.h>
#include <simulator/memory/si_cell.h>
#include <simulator/memory/sram6t_cell.h>

Memory::Memory(int rows, int cols, int row_stride, int col_stride) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0))
{
    this->fun_internal_mask(rows, cols, row_stride, col_stride);
}

std::shared_ptr<Memory> Memory::construct(MemoryType memory_type, int rows, int cols, int row_stride, int col_stride, Config& config) {
    switch(memory_type) {
        case DRAM3T: {
            return std::make_shared<Dram3tCell>(rows, cols, row_stride, col_stride, config);
        }
        case SRAM6T: {
            return std::make_shared<Sram6tCell>(rows, cols, row_stride, col_stride, config);
        }
        case S2I: {
            return std::make_shared<SiCell>(rows, cols, row_stride, col_stride, config);
        };
    }
}

void Memory::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for (int row = 0; row < rows; row += row_stride) {
        for (int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
}

