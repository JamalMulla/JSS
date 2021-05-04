//
// Created by jm1417 on 30/04/2021.
//

#include "scamp5_e.h"

#include <rttr/registration>

void SCAMP5E::init() {
    SCAMP5::init();
    std::shared_ptr<Dram> dram = this->get_component<Dram>("dram");
    if (dram == nullptr) {
        std::cerr << "[Error] Could not get DRAM component. Has it been configured and added to the arhcitecture?" << std::endl;
        exit(EXIT_FAILURE);
    }
    dram_ = dram;
}

// Superpixel methods

void SCAMP5E::set_bitorder(Bitorder bitorder) {
    this->bitorder_ = std::move(bitorder);
    this->superpixel_size_ = this->bitorder_.at(0).size();
    std::vector<std::vector<int>> bank = this->bitorder_.at(0);
    // bits_in_bank is number of non zeros in bank
    this->bits_in_bank_ = 0;

    for (int row = 0; row < superpixel_size_; ++row) {
        for (int col = 0; col < superpixel_size_; ++col) {
            if (bank[row][col] > 0) {
                bits_in_bank_++;
            }
        }
    }
}

void SCAMP5E::superpixel_positions_from_bitorder(position_map& locations) {
    // Locations holds a map from <bank, index> -> x,y coords
    if (bitorder_.empty()) {
        std::cerr << "[Error] Bitorder has not been defined" << std::endl;
        exit(EXIT_FAILURE);
    }
    int banks = bitorder_.size();
    int height = bitorder_[0].size();
    int width = bitorder_[0][0].size();

    for (int b = 0; b < banks; b++) {
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                int index = bitorder_[b][h][w];
                if (index < 1) continue;  // Bitorder indices start 1
                locations[{b, index}] = cv::Point(w, h);
            }
        }
    }
}

void SCAMP5E::superpixel_shift_patterns_from_bitorder(int bank, const std::shared_ptr<DREG>& RNORTH, const std::shared_ptr<DREG>& RSOUTH, const std::shared_ptr<DREG>& REAST, std::shared_ptr<DREG> RWEST, bool shift_left) {
    size_t rows = bitorder_[0].size();
    size_t cols = bitorder_[0][0].size();
    DigitalRegister R_NORTH(rows, cols);
    DigitalRegister R_SOUTH(rows, cols);
    DigitalRegister R_EAST(rows, cols);
    DigitalRegister R_WEST(rows, cols);

    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            int north;
            int west;
            int current = bitorder_[bank][row][col];
            if (current < 1) continue;  // Indices start at 1
            if (row == 0) {
                // No north so set north to current
                north = current;
            } else {
                north = bitorder_[bank][row - 1][col];
            }

            if (col == 0) {
                // No west so set west to current
                west = current;
            } else {
                west = bitorder_[bank][row][col - 1];
            }

            if (current == north + 1) {
                // bigger than north
                (shift_left ? R_SOUTH : R_NORTH).read().at<uint8_t>(row - (shift_left ? 0 : 1), col) = 1;
            } else if (current == north - 1) {
                // smaller than north
                (shift_left ? R_NORTH : R_SOUTH).read().at<uint8_t>(row - (shift_left ? 1 : 0), col) = 1;
            }

            if (current == west + 1) {
                // bigger than west
                (shift_left ? R_EAST : R_WEST).read().at<uint8_t>(row, col - (shift_left ? 0 : 1)) = 1;
            } else if (current == west - 1) {
                // smaller than west
                (shift_left ? R_WEST : R_EAST).read().at<uint8_t>(row, col - (shift_left ? 1 : 0)) = 1;
            }
        }
    }

    switch (origin_) {
        case BOTTOM_LEFT: {
            RSOUTH->write(R_NORTH);
            RNORTH->write(R_SOUTH);
            REAST->write(R_WEST);
            RWEST->write(R_EAST);
            break;
        }
        case BOTTOM_RIGHT: {
            RSOUTH->write(R_NORTH);
            RNORTH->write(R_SOUTH);
            REAST->write(R_EAST);
            RWEST->write(R_WEST);
            break;
        }
        case TOP_LEFT: {
            // opencv default
            RSOUTH->write(R_SOUTH);
            RNORTH->write(R_NORTH);
            REAST->write(R_WEST);
            RWEST->write(R_EAST);
            break;
        }
        case TOP_RIGHT: {
            RSOUTH->write(R_SOUTH);
            RNORTH->write(R_NORTH);
            REAST->write(R_EAST);
            RWEST->write(R_WEST);
            break;
        }
    }
}

void SCAMP5E::superpixel_shift_block(const std::shared_ptr<DREG>& dst, const std::shared_ptr<DREG>& src, const std::shared_ptr<DREG>& RNORTH, const std::shared_ptr<DREG>& RSOUTH, std::shared_ptr<DREG> REAST, const std::shared_ptr<DREG>& RWEST) {
    int rows = src->read().rows;
    int cols = src->read().cols;
    DigitalRegister east = DigitalRegister(rows, cols);
    DigitalRegister north = DigitalRegister(rows, cols);
    DigitalRegister west = DigitalRegister(rows, cols);
    DigitalRegister south = DigitalRegister(rows, cols);

    this->pe->local_read_bus.get_east(east, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_north(north, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_west(west, *src, 1, 0, origin_);
    this->pe->local_read_bus.get_south(south, *src, 1, 0, origin_);

    // todo very messy. improve
    std::shared_ptr<DREG> east_ptr = std::make_shared<DREG>(east);
    std::shared_ptr<DREG> west_ptr = std::make_shared<DREG>(west);
    std::shared_ptr<DREG> north_ptr = std::make_shared<DREG>(north);
    std::shared_ptr<DREG> south_ptr = std::make_shared<DREG>(south);

    AND(east_ptr, east_ptr, REAST);
    AND(north_ptr, north_ptr, RNORTH);
    AND(west_ptr, west_ptr, RWEST);
    AND(south_ptr, south_ptr, RSOUTH);

    OR(dst, east_ptr, north_ptr, south_ptr, west_ptr);
}

void SCAMP5E::superpixel_adc(const std::shared_ptr<DREG>& dst, int bank, std::shared_ptr<AREG> src) {
    // Converts an analogue image to a digital superpixel format
    // Values will always be put in bank 0
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);

    cv::Mat& d = dst->read();
    cv::Mat& sr = src->read();
    parallel_for_(cv::Range(0, sr.rows * sr.cols), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int row = r / sr.cols;
            int col = r % sr.cols;

            if (row % superpixel_size_ != 0) continue;  // Step size is not 1
            if (col % superpixel_size_ != 0) continue;

            int sum = cv::sum(sr(cv::Rect(col, row, superpixel_size_, superpixel_size_)))[0];
            sum /= (superpixel_size_ * superpixel_size_);  // <- this truncates values
            int8_t s = sum;  // TODO Need to have another look at this. Is this correct?
            for (int i = 0; i < bits_in_bank_; i++) {
                int bit = (s >> i) & 1;  // LSB to MSB
                cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
                d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
            }
        }
    });
}

void SCAMP5E::superpixel_dac(std::shared_ptr<AREG> dst, int bank, const std::shared_ptr<DREG>& src) {
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);
    // Converts digital superpixel format image to an analogue image

    cv::Mat& d = dst->read();
    cv::Mat& s = src->read();
    parallel_for_(cv::Range(0, s.rows * s.cols), [&](const cv::Range& range) {
        for (int r = range.start; r < range.end; r++) {
            int row = r / s.cols;
            int col = r % s.cols;

            if (row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
            if (col % superpixel_size_ != 0) continue;

            // Read value from superpixel
            int8_t value = 0;
            for (int i = 0; i < bits_in_bank_; i++) {
                cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
                int bit = s.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
                value |= bit << i;  // LSB to MSB
            }
            d(cv::Rect(col, row, superpixel_size_, superpixel_size_)) = value;
        }
    });
}

void SCAMP5E::superpixel_in(const std::shared_ptr<DREG>& dst, int bank, int value) {
    position_map locations;
    this->superpixel_positions_from_bitorder(locations);
    auto bits = std::make_unique<int[]>(bits_in_bank_);

    // Loads a constant value in each superpixel
    for (int i = 0; i < bits_in_bank_; i++) {
        int bit = (value >> i) & 1;  // LSB to MSB
        bits[i] = bit;
    }

    cv::Mat& d = dst->read();
    for (int col = 0; col < d.cols; col += superpixel_size_) {
        for (int row = 0; row < d.rows; row += superpixel_size_) {
            for (int i = 0; i < bits_in_bank_; i++) {
                cv::Point relative_pos = locations.at({bank, i + 1});
                d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bits[i];
            }
        }
    }
}

void SCAMP5E::superpixel_shift(const std::shared_ptr<DREG>& dst, int bank, const std::shared_ptr<DREG>& src, int shift_left) {
    int rows = src->read().rows;
    int cols = src->read().cols;

    std::shared_ptr<DREG> RNORTH = std::make_shared<DREG>(superpixel_size_, superpixel_size_);
    std::shared_ptr<DREG> RSOUTH = std::make_shared<DREG>(superpixel_size_, superpixel_size_);
    std::shared_ptr<DREG> REAST = std::make_shared<DREG>(superpixel_size_, superpixel_size_);
    std::shared_ptr<DREG> RWEST = std::make_shared<DREG>(superpixel_size_, superpixel_size_);
    superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST,shift_left);

    // TODO non-square superpixels?
    int num_of_repeats_y = rows / superpixel_size_;
    int num_of_repeats_x = cols / superpixel_size_;
    std::shared_ptr<DREG> R_NORTH = std::make_shared<DREG>(cv::repeat(RNORTH->read(), num_of_repeats_y, num_of_repeats_x));
    std::shared_ptr<DREG> R_SOUTH = std::make_shared<DREG>(cv::repeat(RSOUTH->read(), num_of_repeats_y, num_of_repeats_x));
    std::shared_ptr<DREG> R_EAST = std::make_shared<DREG>(cv::repeat(REAST->read(), num_of_repeats_y, num_of_repeats_x));
    std::shared_ptr<DREG> R_WEST = std::make_shared<DREG>(cv::repeat(RWEST->read(), num_of_repeats_y, num_of_repeats_x));
    superpixel_shift_block(dst, src, R_NORTH, R_SOUTH, R_EAST, R_WEST);

    if (bitorder_.size() > 1) {
        // only need to preserve other banks if we have more than 1 bank
        DigitalRegister block_mask = DigitalRegister(superpixel_size_, superpixel_size_);

        cv::Mat& bm = block_mask.read();
        for (size_t b = 0; b < bitorder_.size(); b++) {
            for (size_t row = 0; row < superpixel_size_; row++) {
                for (size_t col = 0; col < superpixel_size_; col++) {
                    int current = bitorder_[b][row][col];
                    if (current < 1) continue;  // Indices start at 1
                    if (b != bank) {
                        // Set mask bit to 1 everywhere other than the bank
                        bm.at<uint8_t>(row, col) = 1;
                    }
                }
            }
        }

        std::shared_ptr<DREG> mask = std::make_shared<DREG>(cv::repeat(bm, num_of_repeats_y, num_of_repeats_x));
        std::shared_ptr<DREG> and_ = std::make_shared<DREG>(rows, cols);

        AND(and_, mask, src);
        OR(dst, and_, dst);
    }
}

void SCAMP5E::superpixel_shift_right(const std::shared_ptr<DREG>& dst, int bank, const std::shared_ptr<DREG>& src) {
    // Logical shift right
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, false);
}
void SCAMP5E::superpixel_shift_left(const std::shared_ptr<DREG>& dst, int bank, const std::shared_ptr<DREG>& src) {
    // Logical shift left
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, true);
}

void SCAMP5E::superpixel_add(const std::shared_ptr<DREG>& dst, int bank, const std::shared_ptr<DREG>& src1, const std::shared_ptr<DREG>& src2) {
    // Clobbers R11
    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>(src1->read().clone());
    std::shared_ptr<DigitalRegister> B = std::make_shared<DigitalRegister>(src2->read().clone());

    std::shared_ptr<DigitalRegister> and_ = std::make_shared<DigitalRegister>(src1->read().rows, src1->read().cols);
    std::shared_ptr<DigitalRegister> xor_ = std::make_shared<DigitalRegister>(src1->read().rows, src1->read().cols);

    AND(and_, A, B);

    while (cv::sum(and_->read())[0] != 0) {
        MOV(R11, A);  // Needed because XOR clobbers A
        XOR(xor_, A, B);
        MOV(A, R11);
        AND(and_, A, B);
        superpixel_shift_left(and_, bank, and_);
        A->write(xor_->read());
        B->write(and_->read());
        AND(and_, A, B);
    }
    MOV(R11, A);
    XOR(dst, A, B);
    MOV(A, R11);
}

//void SCAMP5::superpixel_add(std::shared_ptr<DREG>dst, int bank, std::shared_ptr<DREG> src1, std::shared_ptr<DREG> src2) {
//    position_map locations;
//    this->superpixel_positions_from_bitorder(locations);
//
//    cv::Mat &d = dst->read();
//    cv::Mat &s1 = src1->read();
//    cv::Mat &s2 = src2->read();
//    parallel_for_(cv::Range(0, rows_ * cols_), [&](const cv::Range &range) {
//      for(int r = range.start; r < range.end; r++) {
//          int row = r / cols_;
//          int col = r % cols_;
//
//          if(row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
//          if(col % superpixel_size_ != 0) continue;
//
//          // Read value from each superpixel
//          int value1 = 0;
//          int value2 = 0;
//          for(int i = 0; i < bits_in_bank_; i++) {
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              int bit = s1.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value1 |= bit << i;  // LSB to MSB
//              int bit2 = s2.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value2 |= bit2 << i;  // LSB to MSB
//          }
//
//          int sum = this->array->cla.add(value1, value2);  // Need to have another look at this. Is this correct?
//          for(int i = 0; i < bits_in_bank_; i++) {
//              int bit = (sum >> i) & 1;  // LSB to MSB
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
//          }
//      }
//    });
//#ifdef TRACK_STATISTICS
//    this->array->cla.inc_add();
//#endif
//    this->array->update_cycles(1);
//}

void SCAMP5E::superpixel_sub(const std::shared_ptr<DREG>& dst, int bank, const std::shared_ptr<DREG>& src1, const std::shared_ptr<DREG>& src2) {
    // Clobbers R11

    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>(src1->read().clone());
    std::shared_ptr<DigitalRegister> B = std::make_shared<DigitalRegister>(src2->read().clone());
    std::shared_ptr<DigitalRegister> NOT_A = std::make_shared<DigitalRegister>(src1->read().clone());

    std::shared_ptr<DigitalRegister> and_ = std::make_shared<DigitalRegister>(src1->read().rows, src1->read().cols);
    std::shared_ptr<DigitalRegister> xor_ = std::make_shared<DigitalRegister>(src1->read().rows, src1->read().cols);

    NOT(NOT_A, A);
    AND(and_, NOT_A, B);

    while (cv::sum(and_->read())[0] != 0) {
        MOV(R11, A);  // Needed because XOR clobbers A
        XOR(xor_, A, B);
        MOV(A, R11);
        NOT(NOT_A, A);
        AND(and_, NOT_A, B);
        superpixel_shift_left(and_, bank, and_);
        A->write(xor_->read());
        B->write(and_->read());
        NOT(NOT_A, A);
        AND(and_, NOT_A, B);
    }
    MOV(R11, A);
    XOR(dst, A, B);
    MOV(A, R11);
}

//void SCAMP5::superpixel_sub(std::shared_ptr<DREG>dst, int bank, std::shared_ptr<DREG> src1, std::shared_ptr<DREG> src2) {
//    position_map locations;
//    this->superpixel_positions_from_bitorder(locations);
//
//    cv::Mat &d = dst->read();
//    cv::Mat &s1 = src1->read();
//    cv::Mat &s2 = src2->read();
//    parallel_for_(cv::Range(0, rows_ * cols_), [&](const cv::Range &range) {
//      for(int r = range.start; r < range.end; r++) {
//          int row = r / cols_;
//          int col = r % cols_;
//
//          if(row % superpixel_size_ != 0) continue;  // Step size is superpixel_size_
//          if(col % superpixel_size_ != 0) continue;
//
//          // Read value from each superpixel
//          int value1 = 0;
//          int value2 = 0;
//          for(int i = 0; i < bits_in_bank_; i++) {
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              int bit = s1.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value1 |= bit << i;  // LSB to MSB
//              int bit2 = s2.at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
//              value2 |= bit2 << i;  // LSB to MSB
//          }
//
//          int sum = this->array->cla.add(-value1, value2);  // Need to have another look at this. Is this correct?
//          for(int i = 0; i < bits_in_bank_; i++) {
//              int bit = (sum >> i) & 1;  // LSB to MSB
//              cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
//              d.at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
//          }
//      }
//    });
//#ifdef TRACK_STATISTICS
//    this->array->cla.inc_add();
//    this->array->update_cycles(1);
//#endif
//}

void SCAMP5E::superpixel_movx(const std::shared_ptr<DREG>& dst, std::shared_ptr<DREG> src, news_t dir) {
    for (int i = 0; i < superpixel_size_; ++i) {
        DNEWS(dst, src, dir, 0);
        src = dst;
    }
}

// Histogramming

void SCAMP5E::histogram(const std::shared_ptr<AREG>& src) {
    // Need a block size. There is a tradeoff between the number of arrays and the time it takes as you can only write 1 value at a time
    this->dram_.reset();
    int blocksize = 8;
    int block = 0;
    cv::Mat& s = src->read();

    for (int row = 0; row < rows_; row += blocksize) {
        for (int col = 0; col < cols_; col += blocksize) {
            for (int r = row; r < row + blocksize; r++) {
                for (int c = col; c < col + blocksize; c++) {
                    int m_row = s.at<int16_t>(r, c) + 128;  // intensity is used as index
                    // need to shift by 128 to get a 0-255 range
                    int m_col = 0;

                    int value = this->dram_->read(block, m_row, m_col);
                    this->dram_->write(block, m_row, m_col, value + 1);
                }
            }
            block++;
        }
    }

    // Combine all the dram cells into one by just adding up each of the dram arrays for each value
    std::unordered_map<int, int> histogram;

    for (int i = 0; i < block; i++) {
        // init to 0
        histogram[i] = 0;
    }

    int cells = (rows_ * cols_) / blocksize / blocksize;  // number of dram arrays across array
    for (int i = 0; i < block; i++) {
        int combined = 0;
        for (int b = 0; b < cells; b++) {  //todo parameterise properly
            int i1 = this->dram_->read(b, i, 0);
            combined += i1;
        }

        histogram[i] = combined;
    }

    std::cout << "x = [";
    for (int i = 0; i < block; i++) {
        std::cout << i << ",";
    }
    std::cout << "]" << std::endl;

    std::cout << "y = [";
    for (int i = 0; i < block; i++) {
        std::cout << histogram[i] << ",";
    }
    std::cout << "]" << std::endl;
}

void SCAMP5E::hog(const std::shared_ptr<AREG>& src) {
    // Clobbers A, B
    this->dram_.reset();  //todo time and energy

    scamp5_load_in(F, 127);
    add(src, src, F);

    // calculate gradients gx, gy using sobel
    neg(B, src);
    subx(C, src, west, B);
    subx(E, src, east, B);
    subx(B, B, south, src);
    movx(A, B, north);
    addx(C, C, E, south);
    addx(B, B, A, west);
    sub2x(A, B, east, east, B);
    sub2x(B, C, north, north, C);
    // A = gy
    // B = gx

    cv::Mat A_float;
    cv::Mat B_float;
    A->read().convertTo(A_float, CV_32F, 1 / 255.0);
    B->read().convertTo(B_float, CV_32F, 1 / 255.0);

    // calculate gradient magnitude and direction (in degrees)
    cv::Mat m;  //magnitudes
    cv::Mat a;  //angles
    cv::cartToPolar(A_float, B_float, m, a, true);
    this->update_cycles(350);  // todo better number here for arctan
    // todo also need to add power here that would come from carrying out this computation

    // calculate Histogram of Gradients in 8×8 cells
    int blocksize = 8;
    int bucket_size = 20;
    int num_buckets = 9;

    int block = 0;
    for (int row = 0; row < rows_; row += blocksize) {
        for (int col = 0; col < cols_; col += blocksize) {
            for (int r = row; r < row + blocksize; r++) {
                for (int c = col; c < col + blocksize; c++) {
                    float magnitude = m.at<float>(r, c);
                    int angle = (int)a.at<float>(r, c) % 180;  // unsigned only
                    int m_col = 0;

                    int b1 = floor((double)angle / bucket_size);
                    int b2 = (b1 + 1) % num_buckets;

                    double v2 = ceil(((double)(angle - (b1 * bucket_size)) / bucket_size) * magnitude);
                    double v1 = ceil(magnitude - v2);

                    int value = this->dram_->read(block, b1, m_col);
                    this->dram_->write(block, b1, m_col, value + v1);

                    int value2 = this->dram_->read(block, b2, m_col);
                    this->dram_->write(block, b2, m_col, value2 + v2);
                }
            }
            block++;
        }
    }

    // output histograms for each block
    for (int b = 0; b < block; b++) {
        std::cout << "B:" << b;
        for (int i = 0; i < 10; i++) {  //todo parameterise properly
            int i1 = this->dram_->read(b, i, 0);
            std::cout << ", Bin: " << i * 20 << ", Count: " << i1 << " |";
        }
        std::cout << std::endl;
    }

    // 16×16 Block Normalization
    // Calculate the HOG feature vector
}

rttr::variant SCAMP5E::bitorder_converter(json& j) {
    try {
        auto b = j.get<Bitorder>();
        return b;
    } catch (json::type_error&) {
        std::cerr << "[Warning] Could not parse bitorder. Must be a 3D array" << std::endl;
    } catch (json::parse_error&) {
        std::cerr << "[Warning] Could not parse bitorder. Must be a 3D array" << std::endl;
    }

    return rttr::variant();
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<SCAMP5E>("SCAMP5E")
        .constructor()
        .method("init", &SCAMP5E::init)
        .method("superpixel_positions_from_bitorder", &SCAMP5E::superpixel_positions_from_bitorder)
        .method("superpixel_shift_patterns_from_bitorder", &SCAMP5E::superpixel_shift_patterns_from_bitorder)
        .method("superpixel_adc", &SCAMP5E::superpixel_adc)
        .method("superpixel_dac", &SCAMP5E::superpixel_dac)
        .method("superpixel_in", &SCAMP5E::superpixel_in)
        .method("superpixel_shift", &SCAMP5E::superpixel_shift)
        .method("superpixel_shift_right", &SCAMP5E::superpixel_shift_right)
        .method("superpixel_shift_left", &SCAMP5E::superpixel_shift_left)
        .method("superpixel_add", &SCAMP5E::superpixel_add)
        .method("superpixel_sub", &SCAMP5E::superpixel_sub)
        .method("superpixel_movx", &SCAMP5E::superpixel_movx)
        .method("histogram", &SCAMP5E::histogram)
        .method("hog", &SCAMP5E::hog);
};
