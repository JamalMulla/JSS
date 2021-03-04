//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/array.h"

#include <utility>

Array::Array(int rows, int columns, ProcessingElement pe)
    : rows_(rows), columns_(columns), pe(std::move(pe)) { }

void Array::print_stats(const CycleCounter& counter) {
    this->pe.print_stats(counter);
}

void Array::write_stats(const CycleCounter &counter, json& j) {
    this->pe.write_stats(counter, j);
}
