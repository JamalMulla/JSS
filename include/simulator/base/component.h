//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H


#include <nlohmann/json.hpp>
#include "simulator/metrics/cycle_counter.h"

using json = nlohmann::json;
class Component {
public:
#ifdef TRACK_STATISTICS
    virtual void print_stats(const CycleCounter& counter) = 0;
    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
#endif
};


#endif //SIMULATOR_COMPONENT_H
