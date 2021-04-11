//
// Created by jm1417 on 31/01/2021.
//

#ifndef SIMULATOR_REGISTER_H
#define SIMULATOR_REGISTER_H

#include <simulator/ui/ui.h>

#include <opencv2/core/mat.hpp>

#include "simulator/base/component.h"
#include "simulator/memory/memory.h"

class UI;

class Register : public Component {
   private:
    UI* ui = nullptr;

   protected:
    Memory* memory_ = nullptr;
    cv::Mat value_;
#ifdef TRACK_STATISTICS

#endif
   public:
    std::string name_;
    int min_val = 0;
    int max_val = 0;

    Register(int rows, int columns, int type, Memory& memoryType);
    Register(int rows, int columns, int type);

    void change_memory_type(Memory& memory_type);

#ifdef TRACK_STATISTICS
    void inc_read(const cv::_InputOutputArray& mask = cv::noArray());
    void inc_write(const cv::_InputOutputArray& mask = cv::noArray());

    void update(double time) override;
    cv::Mat& get_static_power() override;
    cv::Mat& get_dynamic_power() override;
    cv::Mat& get_transistor_count() override;
    int get_cycle_count() override;
//    void print_stats(const CycleCounter& counter) override = 0;
//    void write_stats(const CycleCounter& counter, json& j) override = 0;
#endif

    void set_ui_handler(UI* ui_ptr);

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
