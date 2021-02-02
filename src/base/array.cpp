//
// Created by jm1417 on 28/01/2021.
//

#include "array.h"

#include <utility>

Array::Array(int rows, int columns, ProcessingElement pe)
    : rows_(rows), columns_(columns), pe(std::move(pe)) { }

