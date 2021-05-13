//
// Created by jm1417 on 17/04/2021.
//

#ifndef SIMULATOR_DRAM_ARRAY_H
#define SIMULATOR_DRAM_ARRAY_H

#include <array>

#include "simulator/base/component.h"
#include "simulator/base/config.h"
#include <opencv4/opencv2/core.hpp>

class Dram : public Component {
    RTTR_ENABLE(Component);
   private:
    int array_rows_; // rows of each dram array
    int array_cols_; // cols of each dram array. Each column is 1 bit
#ifdef TRACK_STATISTICS
    double time_;
#endif
    // 3 dimensional array of memory cells
    // First dimension is the actual array in the whole plane of arrays
    cv::Mat data;

   public:
    Dram() = default;

    void init();

    void set_array_rows(int array_rows);
    void set_array_cols(int array_cols);

    int read_byte(int array, int row, int start_col);
    void write_byte(int array, int row, int start_col, int value);
    bool read_bit(int array, int row, int col);
    void write_bit(int array, int row, int col, bool value);

    void print_row(int array, int row);

    void reset();

    void update_dynamic(int count);

#ifdef TRACK_STATISTICS

    void update_static(double time) override;
    int get_cycle_count() override;
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;
    double calc_width() override;
    double calc_height() override;
    void print_stats(const CycleCounter& counter) override;
#endif
};


#endif  //SIMULATOR_DRAM_ARRAY_H
