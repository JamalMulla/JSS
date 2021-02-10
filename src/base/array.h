//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARRAY_H
#define SIMULATOR_ARRAY_H


#include <utility>

#include "processing_element.h"
#include "component.h"

#define MAT_TYPE CV_8S

class Array : public Component {
protected:
    int rows_;
    int columns_;

public:
    Array(int rows, int columns, ProcessingElement  pe);

    ProcessingElement pe;

    void print_stats() override;

};


#endif //SIMULATOR_ARRAY_H
