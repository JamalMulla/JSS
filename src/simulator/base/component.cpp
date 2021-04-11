//
// Created by jm1417 on 08/02/2021.
//
#include <simulator/base/component.h>

Component::Component(int rows, int cols, int row_stride, int col_stride) :
    rows_(rows), cols_(cols), row_stride_(row_stride), col_stride_(col_stride) {
}

int Component::get_rows() const {
    return this->rows_;
}

int Component::get_cols() const {
    return this->cols_;
}

int Component::get_row_stride() const {
    return this->row_stride_;
}

int Component::get_col_stride() const {
    return this->col_stride_;
}
