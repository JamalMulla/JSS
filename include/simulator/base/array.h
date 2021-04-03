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

class Array : public Component {
   protected:
    int rows_;
    int columns_;

   public:
    Array(int rows, int columns, ProcessingElement pe);
    /* OpenCV uses a BottomLeft origin.
     * This method converts other coordinate systems to the OpenCV one so that
     * operations on images happen as expected*/

    ProcessingElement pe;
#ifdef TRACK_STATISTICS
    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;
#endif
};

#endif  // SIMULATOR_ARRAY_H
