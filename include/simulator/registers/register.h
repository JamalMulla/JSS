//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <rttr/type>

#include "simulator/base/component.h"
#include "simulator/memory/memory.h"
#include "simulator/metrics/cycle_counter.h"

class Register : public Component {
    RTTR_ENABLE();
   protected:
    std::shared_ptr<Memory> memory_;
    int type_;

   private:
#ifdef USE_CUDA
    cv::cuda::GpuMat value_;
#else
    cv::UMat value_;
#endif

   public:
    std::string name_;
    int min_val = 0;
    int max_val = 0;

    Register() = default;
    void init();

    void change_memory_type(MemoryType memory_type);

#ifdef TRACK_STATISTICS
    // Can only be used if memory_ is defined
    void update_static(double time) override;
    void inc_read(const cv::_InputOutputArray& mask);

    void inc_read();
    void inc_write(const cv::_InputOutputArray& mask);
    void inc_write();
    int get_cycle_count() override;
    void print_stats(const CycleCounter& counter) override = 0;
//    void write_stats(const CycleCounter& counter, json& j) override = 0;

    cv::Mat get_transistor_count_array() override;
    cv::Mat get_static_energy_array() override;
    cv::Mat get_dynamic_energy_array() override;
    double get_width() override;
    double get_height() override;
    int get_transistor_count() override;

#endif

    void set_memory(MemoryType memory_type);
    void set_type(int type);

#ifdef USE_CUDA
    cv::cuda::GpuMat& read();
#else
    cv::UMat& read();
#endif
#ifdef USE_CUDA
    void write(cv::cuda::GpuMat& data);
#else
    void write(cv::UMat& data);
#endif
#ifdef USE_CUDA
    void write(const cv::cuda::GpuMat& data);
#else
    void write(const cv::UMat& data);
#endif
    void write(const cv::Mat& data);
#ifdef USE_CUDA
    void write(cv::cuda::GpuMat& data, cv::cuda::GpuMat& mask);
#else
    void write(cv::UMat& data, cv::UMat& mask);
#endif
#ifdef USE_CUDA
    void write(cv::cuda::GpuMat& data, cv::Mat& mask);
#else
    void write(cv::UMat& data, cv::Mat& mask);
#endif
    void write(Register& data);
    void write(Register& data, Register& mask);
    void write(int data);
#ifdef USE_CUDA
    void write(int data, cv::cuda::GpuMat& mask);
#else
    void write(int data, cv::UMat& mask);
#endif
    void write(int data, Register& mask);

};

#endif  // SIMULATOR_REGISTER_H
