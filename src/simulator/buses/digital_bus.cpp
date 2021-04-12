//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/buses/digital_bus.h"

#include <opencv2/core.hpp>
#include <utility>

#include "../../../tests/utility.h"
#include "simulator/registers/analogue_register.h"

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0,
                    DigitalRegister &d1) {
    // d := d0 OR d1
    // TODO this still isn't enough because the write to d is missed
    // TODO Wrapper around all the opencv functions?
    cv::bitwise_or(d0.read(), d1.read(), d.read());
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
#endif
}

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0,
                    DigitalRegister &d1, DigitalRegister &d2) {
    // d := d0 OR d1 OR d2
    cv::bitwise_or(d0.read(), d1.read(), d0.read());
    cv::bitwise_or(d0.read(), d2.read(), d.read());
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
#endif
}

void DigitalBus::OR(DigitalRegister &d, DigitalRegister &d0,
                    DigitalRegister &d1, DigitalRegister &d2,
                    DigitalRegister &d3) {
    // d := d0 OR d1 OR d2 OR d3
    cv::bitwise_or(d0.read(), d1.read(), d0.read());
    cv::bitwise_or(d0.read(), d2.read(), d0.read());
    cv::bitwise_or(d0.read(), d3.read(), d.read());
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
    d3.inc_write();
#endif
}

void DigitalBus::NOT(DigitalRegister &d, DigitalRegister &d0) {
    // d := NOT d0
    cv::bitwise_xor(d0.read(), 1, d.read());
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_write();
#endif
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0,
                     DigitalRegister &d1) {
    // d := NOT(d0 OR d1)
    DigitalBus::OR(d, d0, d1);
    DigitalBus::NOT(d);
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
#endif
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0,
                     DigitalRegister &d1, DigitalRegister &d2) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::OR(d, d0, d1, d2);
    DigitalBus::NOT(d);
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
#endif
}

void DigitalBus::NOR(DigitalRegister &d, DigitalRegister &d0,
                     DigitalRegister &d1, DigitalRegister &d2,
                     DigitalRegister &d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::OR(d, d0, d1, d2, d3);
    DigitalBus::NOT(d);
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
    d1.inc_read();
    d2.inc_read();
    d3.inc_read();
#endif
}

void DigitalBus::NOT(DigitalRegister &Rl) {
    // Rl := NOT Rl
    cv::bitwise_xor(Rl.read(), 1, Rl.read());
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rl.inc_read();
#endif
}

void DigitalBus::OR(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := Rl OR Rx
    DigitalBus::OR(Rl, Rl, Rx);
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rl.inc_read();
    Rx.inc_read();
#endif
}

void DigitalBus::NOR(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := Rl NOR Rx
    DigitalBus::NOR(Rl, Rl, Rx);
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rl.inc_read();
    Rx.inc_read();
#endif
}

void DigitalBus::AND(DigitalRegister &Ra, DigitalRegister &Rx,
                     DigitalRegister &Ry) {
    // Ra := Rx AND Ry
    cv::bitwise_and(Rx.read(), Ry.read(), Ra.read());
#ifdef TRACK_STATISTICS
    Ra.inc_write();
    Rx.inc_read();
    Ry.inc_read();
#endif
}

void DigitalBus::NAND(DigitalRegister &Ra, DigitalRegister &Rx,
                      DigitalRegister &Ry) {
    // Ra = NOT(Rx AND Ry)
    DigitalBus::AND(Ra, Rx, Ry);
    DigitalBus::NOT(Ra);
#ifdef TRACK_STATISTICS
    Ra.inc_write();
    Rx.inc_read();
    Ry.inc_read();
#endif
}

void DigitalBus::IMP(DigitalRegister &Rl, DigitalRegister &Rx,
                     DigitalRegister &Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOT(intermediate, Ry);
    DigitalBus::OR(Rl, Rx, intermediate);
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
#endif
}

void DigitalBus::NIMP(DigitalRegister &Rl, DigitalRegister &Rx,
                      DigitalRegister &Ry) {
    // Rl := Rx NIMP Ry
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOT(intermediate);
    DigitalBus::NOR(Rl, Rx, intermediate);
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
#endif
}

void DigitalBus::XOR(DigitalRegister &Rl, DigitalRegister &Rx,
                     DigitalRegister &Ry) {
    // Rl := Rx XOR Ry
    cv::bitwise_xor(Rx.read(), Ry.read(), Rl.read());
#ifdef TRACK_STATISTICS
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
#endif
}

void DigitalBus::MOV(DigitalRegister &d, DigitalRegister &d0) {
    // d := d0
    cv::copyTo(d0.read(), d.read(), cv::noArray());
#ifdef TRACK_STATISTICS
    d.inc_write();
    d0.inc_read();
#endif
}

void DigitalBus::MUX(DigitalRegister &Rl, DigitalRegister &Rx,
                     DigitalRegister &Ry, DigitalRegister &Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    // R1 = (Ry.~Rx) + (Rz.Rx)
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalRegister intermediate2(Rl.read().rows, Rl.read().cols);
    cv::bitwise_not(Rx.read(), intermediate.read());
    DigitalBus::AND(intermediate2, Ry, intermediate);
    DigitalBus::AND(intermediate, Rz, Rx);
    DigitalBus::OR(Rl, intermediate, intermediate2);
#ifdef TRACK_STATISTICS
    // TODO fix reads
    Rl.inc_write();
    Rx.inc_read();
    Ry.inc_read();
    Rz.inc_read();
#endif
}

void DigitalBus::CLR_IF(DigitalRegister &Rl, DigitalRegister &Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOT(intermediate, Rl);
    DigitalBus::NOR(Rl, intermediate, Rx);
#ifdef TRACK_STATISTICS
    // TODO fix reads
    Rl.inc_write();
    Rx.inc_read();
#endif
}

// Masked

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                           DigitalRegister &d1, DigitalRegister &FLAG) {
    // d := d0 OR d1
    cv::bitwise_or(d0.read(), d1.read(), d.read(), FLAG.read());
}

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                           DigitalRegister &d1, DigitalRegister &d2,
                           DigitalRegister &FLAG) {
    // d := d0 OR d1 OR d2
    cv::bitwise_or(d0.read(), d1.read(), d0.read(), FLAG.read());
    cv::bitwise_or(d0.read(), d2.read(), d.read(), FLAG.read());
}

void DigitalBus::OR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                           DigitalRegister &d1, DigitalRegister &d2,
                           DigitalRegister &d3, DigitalRegister &FLAG) {
    // d := d0 OR d1 OR d2 OR d3
    cv::bitwise_or(d0.read(), d1.read(), d0.read(), FLAG.read());
    cv::bitwise_or(d0.read(), d2.read(), d0.read(), FLAG.read());
    cv::bitwise_or(d0.read(), d3.read(), d.read(), FLAG.read());
}

void DigitalBus::NOT_MASKED(DigitalRegister &d, DigitalRegister &d0,
                            DigitalRegister &FLAG) {
    // d := NOT d0
    cv::bitwise_xor(d0.read(), 1, d.read(), FLAG.read());
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                            DigitalRegister &d1, DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1)
    DigitalBus::OR_MASKED(d, d0, d1, FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                            DigitalRegister &d1, DigitalRegister &d2,
                            DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1 OR d2)
    DigitalBus::OR_MASKED(d, d0, d1, d2, FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &d, DigitalRegister &d0,
                            DigitalRegister &d1, DigitalRegister &d2,
                            DigitalRegister &d3, DigitalRegister &FLAG) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    DigitalBus::OR_MASKED(d, d0, d1, d2, d3, FLAG);
    DigitalBus::NOT_MASKED(d, FLAG);
}

void DigitalBus::NOT_MASKED(DigitalRegister &Rl, DigitalRegister &FLAG) {
    // Rl := NOT Rl
    cv::bitwise_xor(Rl.read(), 1, Rl.read(), FLAG.read());
}

void DigitalBus::OR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                           DigitalRegister &FLAG) {
    // Rl := Rl OR Rx
    DigitalBus::OR_MASKED(Rl, Rl, Rx, FLAG);
}

void DigitalBus::NOR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                            DigitalRegister &FLAG) {
    // Rl := Rl NOR Rx
    DigitalBus::NOR_MASKED(Rl, Rl, Rx, FLAG);
}

void DigitalBus::AND_MASKED(DigitalRegister &Ra, DigitalRegister &Rx,
                            DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Ra := Rx AND Ry
    cv::bitwise_and(Rx.read(), Ry.read(), Ra.read(), FLAG.read());
}

void DigitalBus::NAND_MASKED(DigitalRegister &Ra, DigitalRegister &Rx,
                             DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Ra = NOT(Rx AND Ry)
    DigitalBus::AND_MASKED(Ra, Rx, Ry, FLAG);
    DigitalBus::NOT_MASKED(Ra, FLAG);
}

void DigitalBus::IMP_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                            DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOT_MASKED(intermediate, Ry, FLAG);
    DigitalBus::OR_MASKED(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::NIMP_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                             DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx NIMP Ry
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOR_MASKED(intermediate, Ry, FLAG);
    DigitalBus::NOR_MASKED(Rl, Rx, intermediate, FLAG);
}

void DigitalBus::XOR_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                            DigitalRegister &Ry, DigitalRegister &FLAG) {
    // Rl := Rx XOR Ry
    cv::bitwise_xor(Rx.read(), Ry.read(), Rl.read(), FLAG.read());
}

void DigitalBus::MOV_MASKED(DigitalRegister &d, DigitalRegister &d0,
                            DigitalRegister &FLAG) {
    // d := d0
    cv::copyTo(d0.read(), d.read(), FLAG.read());
}

void DigitalBus::MUX_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                            DigitalRegister &Ry, DigitalRegister &Rz,
                            DigitalRegister &FLAG) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    // R1 = (Ry.~Rx) + (Rz.Rx)
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalRegister intermediate2(Rl.read().rows, Rl.read().cols);
    cv::bitwise_not(Rx.read(), intermediate.read());
    DigitalBus::AND_MASKED(intermediate2, Ry, intermediate, FLAG);
    DigitalBus::AND_MASKED(intermediate, Rz, Rx, FLAG);
    DigitalBus::OR_MASKED(Rl, intermediate, intermediate2, FLAG);
}

void DigitalBus::CLR_IF_MASKED(DigitalRegister &Rl, DigitalRegister &Rx,
                               DigitalRegister &FLAG) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    DigitalRegister intermediate(Rl.read().rows, Rl.read().cols);
    DigitalBus::NOT_MASKED(intermediate, Rl, FLAG);
    DigitalBus::NOR_MASKED(Rl, intermediate, Rx, FLAG);
}

// Neighbour Operations

void DigitalBus::get_up(DigitalRegister &dst, DigitalRegister &src, int offset,
                        int boundary_fill) {
    // x, y, width, height
    auto read_chunk =
        cv::Rect(0, 0, src.read().cols, src.read().rows - offset);
    auto write_chunk =
        cv::Rect(0, offset, src.read().cols, src.read().rows - offset);
    src.read()(read_chunk).copyTo(dst.read()(write_chunk));
    auto fill = cv::Rect(0, 0, src.read().cols, offset);
    dst.read()(fill).setTo(cv::Scalar(boundary_fill));
#ifdef TRACK_STATISTICS
    src.inc_read();
    dst.inc_write();
#endif
}

void DigitalBus::get_right(DigitalRegister &dst, DigitalRegister &src,
                           int offset, int boundary_fill) {
    // x, y, width, height
    auto read_chunk =
        cv::Rect(offset, 0, src.read().cols - offset, src.read().rows);
    auto write_chunk =
        cv::Rect(0, 0, src.read().cols - offset, src.read().rows);
    src.read()(read_chunk).copyTo(dst.read()(write_chunk));
    auto fill =
        cv::Rect(src.read().cols - offset, 0, offset, src.read().rows);
    dst.read()(fill).setTo(cv::Scalar(boundary_fill));
#ifdef TRACK_STATISTICS
    src.inc_read();
    dst.inc_write();
#endif
}

void DigitalBus::get_left(DigitalRegister &dst, DigitalRegister &src,
                          int offset, int boundary_fill) {
    // x, y, width, height
    auto read_chunk =
        cv::Rect(0, 0, src.read().cols - offset, src.read().rows);
    auto write_chunk =
        cv::Rect(offset, 0, src.read().cols - offset, src.read().rows);
    src.read()(read_chunk).copyTo(dst.read()(write_chunk));
    auto fill = cv::Rect(0, 0, offset, src.read().rows);
    dst.read()(fill).setTo(cv::Scalar(boundary_fill));
#ifdef TRACK_STATISTICS
    src.inc_read();
    dst.inc_write();
#endif
}

void DigitalBus::get_down(DigitalRegister &dst, DigitalRegister &src,
                          int offset, int boundary_fill) {
    // x, y, width, height
    auto read_chunk =
        cv::Rect(0, offset, src.read().cols, src.read().rows - offset);
    auto write_chunk =
        cv::Rect(0, 0, src.read().cols, src.read().rows - offset);
    src.read()(read_chunk).copyTo(dst.read()(write_chunk));
    auto fill =
        cv::Rect(0, src.read().rows - offset, src.read().cols, offset);
    dst.read()(fill).setTo(cv::Scalar(boundary_fill));
#ifdef TRACK_STATISTICS
    src.inc_read();
    dst.inc_write();
#endif
}

void DigitalBus::get_east(DigitalRegister &dst, DigitalRegister &src,
                          int offset, int boundary_fill, Origin origin) {
    switch(origin) {
        case TOP_LEFT:
        case BOTTOM_LEFT: {
            get_right(dst, src, offset, boundary_fill);
            break;
        }
        case TOP_RIGHT:
        case BOTTOM_RIGHT: {
            get_left(dst, src, offset, boundary_fill);
            break;
        }
    }
}

void DigitalBus::get_west(DigitalRegister &dst, DigitalRegister &src,
                          int offset, int boundary_fill, Origin origin) {
    switch(origin) {
        case TOP_LEFT:
        case BOTTOM_LEFT: {
            get_left(dst, src, offset, boundary_fill);
            break;
        }
        case TOP_RIGHT:
        case BOTTOM_RIGHT: {
            get_right(dst, src, offset, boundary_fill);
            break;
        }
    }
}

void DigitalBus::get_north(DigitalRegister &dst, DigitalRegister &src,
                           int offset, int boundary_fill, Origin origin) {
    switch(origin) {
        case BOTTOM_LEFT:
        case BOTTOM_RIGHT: {
            get_up(dst, src, offset, boundary_fill);
            break;
        }
        case TOP_RIGHT:
        case TOP_LEFT: {
            get_down(dst, src, offset, boundary_fill);
            break;
        }
    }
}

void DigitalBus::get_south(DigitalRegister &dst, DigitalRegister &src,
                           int offset, int boundary_fill, Origin origin) {
    switch(origin) {
        case BOTTOM_LEFT:
        case BOTTOM_RIGHT: {
            get_down(dst, src, offset, boundary_fill);
            break;
        }
        case TOP_RIGHT:
        case TOP_LEFT: {
            get_up(dst, src, offset, boundary_fill);
            break;
        }
    }
}

// SuperPixel Operations

void DigitalBus::superpixel_adc(DigitalRegister &dst, int bank, int bits_in_bank, AnalogueRegister &src, position_map &locations, int superpixel_size) {
    // Converts an analogue image to a digital superpixel format
    // Values will always be put in bank 1

    parallel_for_(cv::Range(0, src.read().rows * src.read().cols), [&](const cv::Range &range) {
        for(int r = range.start; r < range.end; r++) {
            int row = r / src.read().cols;
            int col = r % src.read().cols;

            if(row % superpixel_size != 0) continue;  // Step size is not 1
            if(col % superpixel_size != 0) continue;

            int sum = cv::sum(src.read()(cv::Rect(col, row, superpixel_size, superpixel_size)))[0];
            sum /= (superpixel_size * superpixel_size);  // <- this truncates values
            int8_t s = sum;  // Need to have another look at this. Is this correct?
            for(int i = 0; i < bits_in_bank; i++) {
                int bit = (s >> i) & 1;  // LSB to MSB
                cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
                dst.read().at<uint8_t>(relative_pos.y + row, relative_pos.x + col) = bit;
            }
        }
    });
}

void DigitalBus::superpixel_dac(AnalogueRegister &dst, int bank, int bits_in_bank, DigitalRegister &src, position_map &locations, int superpixel_size) {
    // Converts digital superpixel format image to an analogue image

    parallel_for_(cv::Range(0, src.read().rows * src.read().cols), [&](const cv::Range &range) {
        for(int r = range.start; r < range.end; r++) {
            int row = r / src.read().cols;
            int col = r % src.read().cols;

            if(row % superpixel_size != 0) continue;  // Step size is superpixel_size
            if(col % superpixel_size != 0) continue;

            // Read value from superpixel
            int8_t value = 0;
            for(int i = 0; i < bits_in_bank; i++) {
                cv::Point relative_pos = locations.at({bank, i + 1});  // bitorder starts at 1 not 0
                int bit = src.read().at<uint8_t>(relative_pos.y + row, relative_pos.x + col);
                value |= bit << i;  // LSB to MSB
            }
            dst.read()(cv::Rect(col, row, superpixel_size, superpixel_size)) = value;
        }
    });
}

void DigitalBus::superpixel_in(DigitalRegister &dst, int bank, int bits_in_bank, position_map &locations, int superpixel_size, int8_t value) {
    auto bits = std::make_unique<int[]>(bits_in_bank);

    // Loads a constant value in each superpixel
    for(int i = 0; i < bits_in_bank; i++) {
        int bit = (value >> i) & 1;  // LSB to MSB
        bits[i] = bit;
    }

    for(int col = 0; col < dst.read().cols; col += superpixel_size) {
        for(int row = 0; row < dst.read().rows; row += superpixel_size) {
            for(int i = 0; i < bits_in_bank; i++) {
                cv::Point relative_pos = locations.at({bank, i + 1});
                dst.read().at<uint8_t>(relative_pos.y + row,
                                       relative_pos.x + col) = bits[i];
            }
        }
    }
}

void DigitalBus::positions_from_bitorder(
    const std::vector<std::vector<std::vector<int>>> &bitorder, position_map &locations) {
    // Locations holds a map from <bank, index> -> x,y coords
    int banks = bitorder.size();
    int height = bitorder[0].size();
    int width = bitorder[0][0].size();

    for(int b = 0; b < banks; b++) {
        for(int h = 0; h < height; h++) {
            for(int w = 0; w < width; w++) {
                int index = bitorder[b][h][w];
                if(index < 1) continue;  // Bitorder indices start 1
                locations[{b, index}] = cv::Point(w, h);
            }
        }
    }
}

void DigitalBus::superpixel_shift_patterns_from_bitorder(
    int bank, const std::vector<std::vector<std::vector<int>>> &bitorder, DigitalRegister &RN,
    DigitalRegister &RS, DigitalRegister &RE, DigitalRegister &RW,
    bool shift_left, Origin origin) {
    size_t rows = bitorder[0].size();
    size_t cols = bitorder[0][0].size();
    DigitalRegister R_NORTH(rows, cols);
    DigitalRegister R_SOUTH(rows, cols);
    DigitalRegister R_EAST(rows, cols);
    DigitalRegister R_WEST(rows, cols);

    for(size_t row = 0; row < rows; row++) {
        for(size_t col = 0; col < cols; col++) {
            int north;
            int west;
            int current = bitorder[bank][row][col];
            if(current < 1) continue;  // Indices start at 1
            if(row == 0) {
                // No north so set north to current
                north = current;
            } else {
                north = bitorder[bank][row - 1][col];
            }

            if(col == 0) {
                // No west so set west to current
                west = current;
            } else {
                west = bitorder[bank][row][col - 1];
            }

            if(current == north + 1) {
                // bigger than north
                (shift_left ? R_SOUTH : R_NORTH).read().at<uint8_t>(row - (shift_left ? 0 : 1), col) = 1;
            } else if(current == north - 1) {
                // smaller than north
                (shift_left ? R_NORTH : R_SOUTH).read().at<uint8_t>(row - (shift_left ? 1 : 0), col) = 1;
            }

            if(current == west + 1) {
                // bigger than west
                (shift_left ? R_EAST : R_WEST).read().at<uint8_t>(row, col - (shift_left ? 0 : 1)) = 1;
            } else if(current == west - 1) {
                // smaller than west
                (shift_left ? R_WEST : R_EAST).read().at<uint8_t>(row, col - (shift_left ? 1 : 0)) = 1;
            }
        }
    }

    switch(origin) {
        case BOTTOM_LEFT: {
            R_NORTH.read().copyTo(RS.read());
            R_SOUTH.read().copyTo(RN.read());
            R_WEST.read().copyTo(RE.read());
            R_EAST.read().copyTo(RW.read());
            break;
        }
        case BOTTOM_RIGHT: {
            R_NORTH.read().copyTo(RS.read());
            R_SOUTH.read().copyTo(RN.read());
            R_WEST.read().copyTo(RW.read());
            R_EAST.read().copyTo(RE.read());
            break;
        }
        case TOP_LEFT: {
            // opencv default
            R_NORTH.read().copyTo(RN.read());
            R_SOUTH.read().copyTo(RS.read());
            R_WEST.read().copyTo(RE.read());
            R_EAST.read().copyTo(RW.read());
            break;
        }
        case TOP_RIGHT: {
            R_NORTH.read().copyTo(RN.read());
            R_SOUTH.read().copyTo(RS.read());
            R_WEST.read().copyTo(RW.read());
            R_EAST.read().copyTo(RE.read());
            break;
        }
    }
}

void DigitalBus::superpixel_shift_block(DigitalRegister &dst,
                                        DigitalRegister &src, Origin origin,
                                        DigitalRegister &RN,
                                        DigitalRegister &RS,
                                        DigitalRegister &RE,
                                        DigitalRegister &RW) {
    int rows = src.read().rows;
    int cols = src.read().cols;
    DigitalRegister east = DigitalRegister(rows, cols);
    DigitalRegister north = DigitalRegister(rows, cols);
    DigitalRegister west = DigitalRegister(rows, cols);
    DigitalRegister south = DigitalRegister(rows, cols);

    get_east(east, src, 1, 0, origin);
    get_north(north, src, 1, 0, origin);
    get_west(west, src, 1, 0, origin);
    get_south(south, src, 1, 0, origin);

    AND(east, east, RE);
    AND(north, north, RN);
    AND(west, west, RW);
    AND(south, south, RS);

    OR(dst, east, north, south, west);
}

void DigitalBus::superpixel_shift(DigitalRegister &dst, int bank, DigitalRegister &src, const std::vector<std::vector<std::vector<int>>> &bitorder, Origin origin, int shift_left) {
    int superpixel_size = bitorder[0][0].size();  // Assume square superpixel for now
    int rows = src.read().rows;
    int cols = src.read().cols;
    DigitalRegister RN = DigitalRegister(superpixel_size, superpixel_size);
    DigitalRegister RS = DigitalRegister(superpixel_size, superpixel_size);
    DigitalRegister RE = DigitalRegister(superpixel_size, superpixel_size);
    DigitalRegister RW = DigitalRegister(superpixel_size, superpixel_size);
    superpixel_shift_patterns_from_bitorder(bank, bitorder, RN, RS, RE, RW,
                                            shift_left, origin);

    // TODO non-square superpixels?
    int num_of_repeats_y = rows / superpixel_size;
    int num_of_repeats_x = cols / superpixel_size;
    DigitalRegister R_NORTH = cv::repeat(RN.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_SOUTH = cv::repeat(RS.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_EAST = cv::repeat(RE.read(), num_of_repeats_y, num_of_repeats_x);
    DigitalRegister R_WEST = cv::repeat(RW.read(), num_of_repeats_y, num_of_repeats_x);
    superpixel_shift_block(dst, src, origin, R_NORTH, R_SOUTH, R_EAST, R_WEST);

    if(bitorder.size() > 1) {
        // only need to preserve other banks if we have more than 1 bank
        DigitalRegister block_mask = DigitalRegister(superpixel_size, superpixel_size);

        for(size_t b = 0; b < bitorder.size(); b++) {
            for(size_t row = 0; row < superpixel_size; row++) {
                for(size_t col = 0; col < superpixel_size; col++) {
                    int current = bitorder[b][row][col];
                    if(current < 1) continue;  // Indices start at 1
                    if(b != bank) {
                        // Set mask bit to 1 everywhere other than the bank
                        block_mask.read().at<uint8_t>(row, col) = 1;
                    }
                }
            }
        }

        DigitalRegister mask = cv::repeat(block_mask.read(), num_of_repeats_y, num_of_repeats_x);
        DigitalRegister and_ = DigitalRegister(rows, cols);

        AND(and_, mask, src);
        OR(dst, and_, dst);
    }
}

void DigitalBus::superpixel_shift_left(DigitalRegister &dst, int bank, DigitalRegister &src, const std::vector<std::vector<std::vector<int>>> &bitorder, Origin origin) {
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, bitorder, origin, true);
}

void DigitalBus::superpixel_shift_right(DigitalRegister &dst, int bank, DigitalRegister &src, const std::vector<std::vector<std::vector<int>>> &bitorder, Origin origin) {
    // src and dst cannot be the same
    superpixel_shift(dst, bank, src, bitorder, origin, false);
}

void DigitalBus::superpixel_add(DigitalRegister &dst, int bank, DigitalRegister &src1,
                                DigitalRegister &src2, const std::vector<std::vector<std::vector<int>>> &bitorder, Origin origin) {
    DigitalRegister A = src1.read().clone();
    DigitalRegister B = src2.read().clone();
    DigitalRegister and_ = DigitalRegister(src1.read().rows, src1.read().cols);
    DigitalRegister xor_ = DigitalRegister(src1.read().rows, src1.read().cols);

    AND(and_, A, B);

    while(cv::sum(and_.read())[0] != 0) {
        XOR(xor_, A, B);
        AND(and_, A, B);
        superpixel_shift_left(and_, bank, and_, bitorder, origin);
        xor_.read().copyTo(A.read());
        and_.read().copyTo(B.read());
        AND(and_, A, B);
    }
    XOR(dst, A, B);
}

void DigitalBus::superpixel_sub(DigitalRegister &dst, int bank, DigitalRegister &src1,
                                DigitalRegister &src2, const std::vector<std::vector<std::vector<int>>> &bitorder, Origin origin) {
    DigitalRegister A = src1.read().clone();
    DigitalRegister B = src2.read().clone();
    DigitalRegister NOT_A = src1.read().clone();
    DigitalRegister and_ =
        DigitalRegister(src1.read().rows, src1.read().cols);
    DigitalRegister xor_ =
        DigitalRegister(src1.read().rows, src1.read().cols);

    NOT(NOT_A, A);
    AND(and_, NOT_A, B);

    while(cv::sum(and_.read())[0] != 0) {
        XOR(xor_, A, B);
        NOT(NOT_A, A);
        AND(and_, NOT_A, B);
        superpixel_shift_left(and_, bank, and_, bitorder, origin);
        xor_.read().copyTo(A.read());
        and_.read().copyTo(B.read());
        NOT(NOT_A, A);
        AND(and_, NOT_A, B);
    }
    XOR(dst, A, B);
}
