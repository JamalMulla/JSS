//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARRAY_H
#define SIMULATOR_ARRAY_H


#include <utility>

#include "processing_element.h"

class Array {
protected:
    int rows;
    int columns;
    ProcessingElement pe;

public:
    Array(int rows, int columns, ProcessingElement pe)
        : rows(rows), columns(columns), pe(std::move(pe))
    {

    }
};


#endif //SIMULATOR_ARRAY_H
