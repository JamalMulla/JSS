//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PIXEL_H
#define SIMULATOR_PIXEL_H

#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/videoio.hpp>

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
    double static_power_;  // in watts
    double dynamic_power_;  // in watts
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
    cv::Mat internal_mask;
    std::shared_ptr<Config> config_;

    void fun_internal_mask(int rows, int cols, int row_stride, int col_stride);
#endif

   public:
    Pixel(int rows, int cols, int row_stride, int col_stride, Source src, const std::string& path, std::shared_ptr<Config> config);
    void reset();
    void read(Register& reg);
    double last_frame_time();
#ifdef TRACK_STATISTICS
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    cv::Mat get_transistor_count() override;
    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;
#endif
    ~Pixel();
};

#endif  // SIMULATOR_PIXEL_H
