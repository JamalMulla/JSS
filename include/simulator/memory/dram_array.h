//
// Created by jm1417 on 17/04/2021.
//

#ifndef SIMULATOR_DRAM_ARRAY_H
#define SIMULATOR_DRAM_ARRAY_H

#include <array>

#include "simulator/base/component.h"
#include "simulator/base/config.h"
#include <opencv2/core.hpp>


// Cols are in terms of word length
class Dram : public Component {
   private:
    int rows_; // rows of the whole plane
    int cols_; // cols of the whole plane
    int row_stride_;
    int col_stride_;
    int array_rows_; // rows of each dram array
    int array_cols_; // cols of each dram array
    int word_length_;
    int cycle_count_ = 1;
    int transistor_count_;
    double static_power_;  // in Watts
    double dynamic_power_;  // in Watts
    cv::Mat internal_mask;  // Used to keep track of components in array when stride is not 1, i.e. spaces between components
    cv::Mat array_transistor_count_;
    cv::Mat array_static_energy_;
    cv::Mat array_dynamic_energy_;
    // 3 dimensional array of memory cells
    // First dimension is the actual array in the whole plane of arrays
    // Word addressable. Word is 8 bits by default. Can be changed
    cv::Mat data;

    void fun_internal_mask();
    int fun_transistor(const Config& config);
    double fun_static(const Config& config);
    double fun_dynamic(const Config& config);

   public:
    Dram(int rows, int cols, int row_stride, int col_stride, int array_rows, int array_cols, int word_length, const Config& config);
    int read(int array, int row, int col);
    void write(int array, int row, int col, int value);
    void reset();

#ifdef TRACK_STATISTICS
    void update_static(double time) override;
    int get_cycle_count() override;
    cv::Mat& get_static_energy() override;
    cv::Mat& get_dynamic_energy() override;
    cv::Mat& get_transistor_count() override;
    void print_stats(const CycleCounter& counter) override;
#endif
};


#endif  //SIMULATOR_DRAM_ARRAY_H
