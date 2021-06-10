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
    RTTR_ENABLE(Component);
   protected:
    int read_count_ = 0;
    int write_count_ = 0;
    double dynamic_read_power_;  // in Watts for a read
    double dynamic_write_power_;  // in Watts for a read
    double time_; // time in seconds for a read/write
#ifdef USE_CUDA
    cv::cuda::GpuMat scratch;
#else
    cv::UMat scratch;
#endif

   public:
    Memory() = default;
    virtual void init();

#ifdef TRACK_STATISTICS
    virtual void read(const cv::_InputOutputArray &mask);
    virtual void read();
    virtual void write(const cv::_InputOutputArray &mask);
    virtual void write();

    virtual double calc_dynamic_read() = 0;
    virtual double calc_dynamic_write() = 0;

    cv::Mat get_dynamic_energy_array() override;
    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter& counter) override;
#endif
    static std::shared_ptr<Memory> construct(MemoryType memory_type, int rows, int cols, int row_stride, int col_stride, const std::shared_ptr<Config>& config);
};


#endif  // SIMULATOR_MEMORY_H
