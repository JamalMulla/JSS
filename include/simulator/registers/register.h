//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <opencv4/opencv2/core/mat.hpp>

#include "simulator/base/component.h"
#include "simulator/metrics/cycle_counter.h"
#include "simulator/memory/memory.h"

#include <rttr/type>

class Register : public Component {
    RTTR_ENABLE();
   protected:
    std::shared_ptr<Memory> memory_;
    int type_;

   private:
    cv::Mat value_;

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

    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
#endif

    void set_memory(MemoryType memory_type);
    void set_type(int type);

    cv::Mat get_transistor_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    cv::Mat& read();
    void write(cv::Mat& data);
    void write(const cv::Mat& data);
    void write(cv::Mat& data, cv::Mat& mask);
    void write(Register& data);
    void write(Register& data, Register& mask);
    void write(int data);
    void write(int data, cv::Mat& mask);
    void write(int data, Register& mask);

};

#endif  // SIMULATOR_REGISTER_H
