//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H


#include "../metrics/cycle_counter.h"

class Component {
public:

    virtual void print_stats(CycleCounter counter) = 0;
};


#endif //SIMULATOR_COMPONENT_H
