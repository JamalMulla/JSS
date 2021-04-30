//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARRAY_H
#define SIMULATOR_ARRAY_H

#include <simulator/adders/cla.h>
#include <simulator/memory/dram_array.h>
#include <simulator/metrics/stats_outputter.h>

#include <functional>

#include "component.h"
#include "plane_params.h"
#include "processing_element.h"

// TODO implement StatsOutputter
class Array {
   private:
    int rows_;
    int columns_;
    Config config_;
    CycleCounter counter_;

   public:
    Array(int rows, int columns, Config& config, ProcessingElement pe);
    std::vector<Component> layers;

    ProcessingElement pe;
//    CarryLookAheadAdder cla;
//    Dram dram;

    void update_cycles(int cycles);
#ifdef TRACK_STATISTICS
    void update_static(); // Calculates static power using the number of cycles we've got to
    void print_stats();
    unsigned long long get_cycles();
#endif
};

#endif  // SIMULATOR_ARRAY_H
