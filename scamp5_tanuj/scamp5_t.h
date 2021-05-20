//
// Created by jm1417 on 30/04/2021.
//

#ifndef SIMULATOR_SCAMP5_T_H
#define SIMULATOR_SCAMP5_T_H

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

/*Tanuj SCAMP5*/

class SCAMP5T : public SCAMP5 {
   RTTR_ENABLE(SCAMP5);
   private:
    Bitorder bitorder_;
    int superpixel_size_;
    int bits_in_bank_;
    std::shared_ptr<Dram> dram_;

   public:
    SCAMP5T() = default;
    void init();
    rttr::variant bitorder_converter(json &j);

    // Superpixel methods
    void set_bitorder(Bitorder bitorder);

    void superpixel_positions_from_bitorder(position_map &locations);
    void superpixel_shift_patterns_from_bitorder(int bank, const std::shared_ptr<DREG>&RNORTH,
                                                 const std::shared_ptr<DREG>&RSOUTH, const std::shared_ptr<DREG>&REAST, const std::shared_ptr<DREG>&RWEST,
                                                 bool shift_left);
    void superpixel_shift_block(const std::shared_ptr<DREG>&dst, const std::shared_ptr<DREG>&src,
                                const std::shared_ptr<DREG>&RNORTH,
                                const std::shared_ptr<DREG>&RSOUTH, const std::shared_ptr<DREG>&REAST,
                                const std::shared_ptr<DREG>&RWEST);
    void superpixel_adc(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<AREG>&src);
    void superpixel_dac(const std::shared_ptr<AREG>&dst, int bank, const std::shared_ptr<DREG>&src);
    void superpixel_in(const std::shared_ptr<DREG>&dst, int bank, int value);
    void superpixel_shift(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<DREG>&src, int shift_left);
    void superpixel_shift_right(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<DREG>&src);
    void superpixel_shift_left(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<DREG>&src);
    void superpixel_add(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<DREG>&src1, const std::shared_ptr<DREG>&src2);
    void superpixel_sub(const std::shared_ptr<DREG>&dst, int bank, const std::shared_ptr<DREG>&src1, const std::shared_ptr<DREG>&src2);
    void superpixel_movx(const std::shared_ptr<DREG>&dst, std::shared_ptr<DREG>src, news_t dir);

    // Histogramming
    void histogram(const std::shared_ptr<AREG>& src);
    void hog(const std::shared_ptr<AREG>& src);

};


#endif  //SIMULATOR_SCAMP5_T_H
