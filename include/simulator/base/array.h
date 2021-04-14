//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARRAY_H
#define SIMULATOR_ARRAY_H

#include <simulator/adders/cla.h>
#include <simulator/metrics/stats_outputter.h>

#include <functional>

#include "component.h"
#include "plane_params.h"
#include "processing_element.h"

#define MAT_TYPE CV_16S

// TODO implement StatsOutputter
class Array {
   private:
    int rows_;
    int columns_;
    Config* config_;
    CycleCounter counter_;

   public:
    Array(int rows, int columns, Config& config, ProcessingElement pe);

    ProcessingElement pe;
    CarryLookAheadAdder cla;
    void update_cycles(int cycles);
#ifdef TRACK_STATISTICS
    void print_stats();
    unsigned long long get_cycles();
#endif
};

#endif  // SIMULATOR_ARRAY_H
