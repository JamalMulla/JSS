//
// Created by jm1417 on 30/04/2021.
//

#ifndef SIMULATOR_SCAMP5_E_H
#define SIMULATOR_SCAMP5_E_H

#include "../scamp5/scamp5.h"

struct pair_hash {
    template<class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
        auto lhs = p.first;
        auto rhs = p.second;
        lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
        return lhs;
    }
};

using bank_index = std::pair<int, int>;
using position_map = std::unordered_map<bank_index, cv::Point, pair_hash>;
using Bitorder = std::vector<std::vector<std::vector<int>>>;

/*Extended SCAMP5*/

class SCAMP5E : public SCAMP5 {
   private:
    Bitorder bitorder_;
    int superpixel_size_;
    int bits_in_bank_;
    std::shared_ptr<Dram> dram_;

   public:
    class builder;
    SCAMP5E(int rows, int cols, Origin origin);
    // Superpixel methods
    void set_bitorder(Bitorder bitorder);

    void superpixel_positions_from_bitorder(position_map &locations);
    void superpixel_shift_patterns_from_bitorder(int bank, DREG *RNORTH,
                                                 DREG *RSOUTH, DREG *REAST, DREG *RWEST,
                                                 bool shift_left);
    void superpixel_shift_block(DREG *dst, DREG *src,
                                DREG *RNORTH,
                                DREG *RSOUTH, DREG *REAST,
                                DREG *RWEST);
    void superpixel_adc(DREG *dst, int bank, AREG *src);
    void superpixel_dac(AREG *dst, int bank, DREG *src);
    void superpixel_in(DREG *dst, int bank, int value);
    void superpixel_shift(DREG *dst, int bank, DREG *src, int shift_left);
    void superpixel_shift_right(DREG *dst, int bank, DREG *src);
    void superpixel_shift_left(DREG *dst, int bank, DREG *src);
    void superpixel_add(DREG *dst, int bank, DREG *src1, DREG *src2);
    void superpixel_sub(DREG *dst, int bank, DREG *src1, DREG *src2);
    void superpixel_movx(DREG *dst, DREG *src, news_t dir);

    // Histogramming
    void histogram(AREG *src);
    void hog(AREG *src);
    RTTR_ENABLE();
};

class SCAMP5E::builder {
    RTTR_ENABLE();

   private:
    int rows_ = 256;
    int cols_ = 256;
    Origin origin_ = Origin::TOP_LEFT;
    Bitorder bitorder_;

   public:
    rttr::variant bitorder_converter(json &j);
    builder &with_rows(int rows);
    builder &with_cols(int cols);
    builder &with_origin(Origin origin);
    builder &with_bitorder(Bitorder bitorder);

    SCAMP5E build();
};

#endif  //SIMULATOR_SCAMP5_E_H
