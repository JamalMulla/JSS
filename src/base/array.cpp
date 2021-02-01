//
// Created by jm1417 on 28/01/2021.
//

#include "array.h"

Array::Array(int rows, int columns, ProcessingElement pe)
    : rows(rows), columns(columns), pe(std::move(pe)) { }
