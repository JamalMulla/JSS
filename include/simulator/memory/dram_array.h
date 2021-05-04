//
// Created by jm1417 on 17/04/2021.
//

#ifndef SIMULATOR_DRAM_ARRAY_H
#define SIMULATOR_DRAM_ARRAY_H

#include <array>

#include "simulator/base/component.h"
#include "simulator/base/config.h"
#include <opencv4/opencv2/core.hpp>


// Cols are in terms of word length
class Dram : public Component {
   private:
    int rows_; // rows of the whole plane
    int cols_; // cols of the whole plane
    int row_stride_ = 1;
    int col_stride_ = 1;
    int array_rows_; // rows of each dram array
    int array_cols_; // cols of each dram array
    int word_length_;
    std::shared_ptr<Config> config_;
#ifdef TRACK_STATISTICS
    int cycle_count_ = 1;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
#endif
    // 3 dimensional array of memory cells
    // First dimension is the actual array in the whole plane of arrays
    // Word addressable. Word is 8 bits by default. Can be changed
    cv::Mat data;

   public:
    Dram() = default;

    void init();

    void set_rows(int rows);
    void set_cols(int cols);
    void set_row_stride(int row_stride);
    void set_col_stride(int col_stride);
    void set_array_rows(int array_rows);
    void set_array_cols(int array_cols);
    void set_word_length(int word_length);
    void set_config(const std::shared_ptr<Config>& config);

    int read(int array, int row, int col);
    void write(int array, int row, int col, int value);
    void reset();

#ifdef TRACK_STATISTICS
    void fun_internal_mask();
    int fun_transistor(const std::shared_ptr<Config>& config);
    double fun_static(const std::shared_ptr<Config>& config);
    double fun_dynamic(const std::shared_ptr<Config>& config);
    void update_static(double time) override;
    int get_cycle_count() override;
    cv::Mat get_static_energy() override;
    cv::Mat get_dynamic_energy() override;
    cv::Mat get_transistor_count() override;
    void print_stats(const CycleCounter& counter) override;
#endif
};


#endif  //SIMULATOR_DRAM_ARRAY_H
