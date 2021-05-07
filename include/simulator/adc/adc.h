//
// Created by jm1417 on 07/05/2021.
//

#ifndef SIMULATOR_ADC_H
#define SIMULATOR_ADC_H


#include "simulator/base/component.h"
#include "simulator/base/config.h"

class ADC : public Component {
   private:
    int rows_;
    int cols_;
    int row_stride_ = 1;
    int col_stride_ = 1;
    std::shared_ptr<Config> config_;
    int cycle_count_;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    double time_;
    cv::Mat internal_mask;
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
    int fun_transistor(int bits, Config& config);
    double fun_static(int bits, Config& config);
    double fun_dynamic(int bits, Config& config);
    void fun_internal_mask(int rows, int cols, int row_stride, int col_stride);

   public:

   public:
    ADC() = default;
    void init();

    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_config(const std::shared_ptr<Config>& config);

    int convert(int analogue);

#ifdef TRACK_STATISTICS
    void fun_internal_mask();
    int fun_transistor(const std::shared_ptr<Config>& config);
    double fun_static(const std::shared_ptr<Config>& config);
    double fun_dynamic(const std::shared_ptr<Config>& config);
    void update_static(double time) override;
    void update_dynamic(int count);
    cv::Mat get_transistor_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter &counter) override;
#endif
};

#endif  //SIMULATOR_ADC_H
