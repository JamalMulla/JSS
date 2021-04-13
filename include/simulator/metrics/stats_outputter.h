//
// Created by jm1417 on 13/04/2021.
//

#ifndef SIMULATOR_STATS_OUTPUTTER_H
#define SIMULATOR_STATS_OUTPUTTER_H

#include "cycle_counter.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

/*Interface for outputting of metrics*/
class StatsOutputter {
#ifdef TRACK_STATISTICS
virtual void print_stats(const CycleCounter& counter) = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
#endif
};

#endif  //SIMULATOR_STATS_OUTPUTTER_H
