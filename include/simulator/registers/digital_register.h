//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_DIGITAL_REGISTER_H
#define SIMULATOR_DIGITAL_REGISTER_H

#include <opencv4/opencv2/core/mat.hpp>
#include <rttr/type>
#include "register.h"
#include "simulator/memory/dram3t_cell.h"

class DigitalRegister : public Register {
    RTTR_ENABLE(Register);
    std::shared_ptr<cv::Mat> mask_ = std::make_shared<cv::Mat>(cv::noArray().getMat_());

   public:
    DigitalRegister(int rows, int columns, const std::shared_ptr<Config>& config, int row_stride = 1, int col_stride = 1, MemoryType memory_type = MemoryType::DRAM3T);
    DigitalRegister(int rows, int cols, int row_stride = 1, int col_stride = 1);
    DigitalRegister(const cv::Mat& data, int row_stride = 1, int col_stride = 1);

    DigitalRegister& operator()(const std::string& name);

    void set_mask(const std::shared_ptr<DigitalRegister>& mask);
    cv::Mat& get_mask();
    void set();
    void clear();

#ifdef TRACK_STATISTICS
    void print_stats(const CycleCounter& counter) override;
//    void write_stats(const CycleCounter& counter, json& j) override;

#endif
};

#endif  // SIMULATOR_DIGITAL_REGISTER_H
