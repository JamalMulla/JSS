//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <opencv2/core.hpp>
#include "simulator/base/component.h"
#include "simulator/base/config.h"

enum MemoryType {
    DRAM3T,
    SRAM6T,
    S2I
};

class Memory : public Component {
   public:
    Memory() = default;
    virtual void init() = 0;

#ifdef TRACK_STATISTICS
    virtual void read(const cv::_InputOutputArray &mask) = 0;
    virtual void read() = 0;
    virtual void write(const cv::_InputOutputArray &mask) = 0;
    virtual void write() = 0;
#endif
    static std::shared_ptr<Memory> construct(MemoryType memory_type, int rows, int cols, int row_stride, int col_stride, const std::shared_ptr<Config>& config);
};


#endif  // SIMULATOR_MEMORY_H
