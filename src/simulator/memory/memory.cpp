//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/memory/memory.h"

Memory::Memory(int rows, int cols, int row_stride, int col_stride) :
    rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    internal_mask(rows, cols, CV_8U, cv::Scalar(0))
{
    this->fun_internal_mask(rows, cols, row_stride, col_stride);
}

void Memory::fun_internal_mask(int rows, int cols, int row_stride, int col_stride) {
    for (int row = 0; row < rows; row += row_stride) {
        for (int col = 0; col < cols; col += col_stride) {
            this->internal_mask.at<uint8_t>(row, col) = 1;
        }
    }
}
