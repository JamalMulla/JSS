//
// Created by jm1417 on 28/01/2021.
//

#include "array.h"

Array::Array(int rows, int columns, const ProcessingElement& pe)
    : rows_(rows), columns_(columns), pe(std::move(pe)) { }

void Array::print_stats(CycleCounter counter) {
    this->pe.print_stats(counter);
}
