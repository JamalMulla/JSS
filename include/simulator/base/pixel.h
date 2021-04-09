//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PIXEL_H
#define SIMULATOR_PIXEL_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "config.h"
#include "simulator/base/component.h"
#include "simulator/input/input_source.h"
#include "simulator/registers/analogue_register.h"

class Pixel : public Component {
   private:
    std::shared_ptr<InputSource> input_source;
#ifdef TRACK_STATISTICS
    int cycle_count_;
    int transistor_count_;
    double static_power_; // in watts
    double dynamic_power_; // in watts
    Config* config_;
#endif

   public:
    Pixel(int rows, int cols, Source src, const std::string& path, Config& config);
    void reset();
    void read(Register& reg);
    double last_frame_time();
#ifdef TRACK_STATISTICS
    double get_static_power(double time) override;
    double get_dynamic_power() override;
    int get_cycle_count() override;
    int get_transistor_count() override;
    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;
#endif
    ~Pixel();
};

#endif  // SIMULATOR_PIXEL_H
