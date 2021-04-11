//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARRAY_H
#define SIMULATOR_ARRAY_H

#include <functional>

#include "component.h"
#include "plane_params.h"
#include "processing_element.h"

#define MAT_TYPE CV_16S

class Array {
   private:
    CycleCounter counter;

   protected:
    int rows_;
    int columns_;

   public:
    Array(int rows, int columns, ProcessingElement pe);
    void update_cycles(int cycles);

    ProcessingElement pe;
#ifdef TRACK_STATISTICS
//    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_ARRAY_H
