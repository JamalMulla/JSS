//
// Created by jm1417 on 05/05/2021.
//

#include "scamp5rm.h"

#include <simulator/adc/adc.h>
#include <simulator/external/parser.h>
#include <simulator/memory/sram6t_cell.h>
#include <simulator/ui/ui.h>
#include <simulator/util/utility.h>

#include <filesystem>
#include <iostream>
#include <ostream>
#include <rttr/registration>
#include <utility>

void SCAMP5RM::init() {
    // Registers used often in instructions
    pe = this->get_component<ProcessingElement>("pe");
    alu = this->get_component<ALU>("alu");
    dram = this->get_component<Dram>("dram");
    adc = this->get_component<ADC>("adc");

    // TODO write to DRAM - Initially all PEs are active
//    FLAG->write(1);

//    intermediate_a = std::make_shared<AREG>(this->rows_, this->cols_);
//    intermediate_a2 = std::make_shared<AREG>(this->rows_, this->cols_);
//    intermediate_d = std::make_shared<DREG>(this->rows_, this->cols_);
}

void SCAMP5RM::nop() { this->update_cycles(1); }

void SCAMP5RM::rpix() {
    // reset *PIX
    this->pe->get_pixel()->reset();
    this->update_cycles(1);
}

// TODO ADC
void SCAMP5RM::get_image(AREG y) {
    // y := half-range image, and reset pixel
    cv::Mat image = this->pe->get_pixel()->read();

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int16_t v = image.at<int16_t>(row, col);
            dram->write_byte(row, col, PIX, v/2);
            dram->write_byte(row, col, NEWS, -v/2);
            dram->write_byte(row, col, y, v/2);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    adc->update_dynamic(cols_);
    this->update_cycles(cols_ * 8 * 3);
    this->rpix();
    this->rpix();
    this->nop();
}

void SCAMP5RM::get_image(AREG y, AREG h) {
    // y := full-range image, h := negative half-range image, and reset *PIX
    cv::Mat image = this->pe->get_pixel()->read();

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int16_t v = image.at<int16_t>(row, col);
            dram->write_byte(row, col, PIX, v/2);
            dram->write_byte(row, col, NEWS, -v/2);
            dram->write_byte(row, col, h, -v/2);
            dram->write_byte(row, col, y, v);
        }
    }
    this->update_cycles(cols_ * 8 * 4);
    adc->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->rpix();
    this->rpix();
    this->nop();
}
//
//void SCAMP5RM::bus(AREG a) {
//    // a = 0 + error
//    this->pe->analogue_bus.bus(*a, *FLAG);
//    this->update_cycles(1);
//}
//
//void SCAMP5RM::bus(AREG a, AREG a0) {
//    // a = -a0 + error
//    this->pe->analogue_bus.bus(*a, *a0, *FLAG);
//    this->update_cycles(1);
//}
//
//void SCAMP5RM::bus(AREG a, AREG a0, AREG a1) {
//    // a = -(a0 + a1) + error
//    this->pe->analogue_bus.bus(*a, *a0, *a1, *FLAG);
//    this->update_cycles(4); // 2 reads, 1 add, 1 write
//}
//
//void SCAMP5RM::bus(AREG a, AREG a0, AREG a1, AREG a2) {
//    // a = -(a0 + a1 + a2) + error
//    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *FLAG);
//    this->update_cycles(5);  // 3 reads, 1 add, 1 write
//}
//
//void SCAMP5RM::bus(AREG a, AREG a0, AREG a1, AREG a2, AREG a3) {
//    // a = -(a0 + a1 + a2 + a3) + error
//    this->pe->analogue_bus.bus(*a, *a0, *a1, *a2, *a3, *FLAG);
//    this->update_cycles(6);  // 4 reads, 1 add, 1 write
//}
//
//void SCAMP5RM::bus2(AREG a, AREG b) {
//    // a,b = 0 + error
//    this->pe->analogue_bus.bus2(*a, *b, *FLAG);
//    this->update_cycles(2);  // 2 writes
//}
//
//void SCAMP5RM::bus2(AREG a, AREG b, AREG a0) {
//    // a,b = -0.5*a0 + error + noise
//    this->pe->analogue_bus.bus2(*a, *b, *a0, *FLAG);
//    this->update_cycles(3);  // 1 read, 2 writes
//}
//
//void SCAMP5RM::bus2(AREG a, AREG b, AREG a0, AREG a1) {
//    // a,b = -0.5*(a0 + a1) + error + noise
//    this->pe->analogue_bus.bus2(*a, *b, *a0, *a1, *FLAG);
//    this->update_cycles(5);  // 2 reads, 1 add, 2 writes
//}
//
//void SCAMP5RM::bus3(AREG a, AREG b, AREG c, AREG a0) {
//    // a,b,c = -0.33*a0 + error + noise
//    this->pe->analogue_bus.bus3(*a, *b, *c, *a0, *FLAG);
//    this->update_cycles(2);  // 1 read, 3 writes
//}

void SCAMP5RM::where(AREG a) {
    // FLAG := a > 0

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int val = this->dram->read_byte(row, col, a);
            this->alu->execute(val, 0, ALU::CMP);
            if (!this->alu->N) {
                this->dram->write_bit(row, col, FLAG, true);
            } else {
                this->dram->write_bit(row, col, FLAG, false);
            }
        }
    }
    this->update_cycles(cols_ * 10);
    dram->update_dynamic(cols_ * (8 + 1));
    alu->update_dynamic(cols_);
}

// TODO use alu for comparison
void SCAMP5RM::where(AREG a0, AREG a1) {
    // FLAG := (a0 + a1) > 0.

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int a0_val = this->dram->read_byte(row, col, a0);
            int a1_val = this->dram->read_byte(row, col, a1);
            int res = this->alu->execute(a0_val, a1_val, ALU::ADD);
            this->alu->execute(res, 0, ALU::CMP);
            if (!this->alu->N) {
                this->dram->write_bit(row, col, FLAG, true);
            } else {
                this->dram->write_bit(row, col, FLAG, false);
            }
        }
    }
    this->update_cycles(cols_ * 10);
    dram->update_dynamic(cols_ * 17);
    alu->update_dynamic(cols_ * 2);
}

// TODO use alu for comparison
void SCAMP5RM::where(AREG a0, AREG a1, AREG a2) {
    // FLAG := (a0 + a1 + a2) > 0.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int a0_val = this->dram->read_byte(row, col, a0);
            int a1_val = this->dram->read_byte(row, col, a1);
            int a2_val = this->dram->read_byte(row, col, a2);
            int middle = this->alu->execute(a0_val, a1_val, ALU::ADD);
            int res = this->alu->execute(middle, a2_val, ALU::ADD);
            this->alu->execute(res, 0, ALU::CMP);
            if (!this->alu->N) {
                this->dram->write_bit(row, col, FLAG, true);
            } else {
                this->dram->write_bit(row, col, FLAG, false);
            }
        }
    }
    this->update_cycles(cols_ * 28);
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 25);
}

// TODO Dram write
void SCAMP5RM::all() {
    // FLAG := 1.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, FLAG, 1);
        }
    }
    dram->update_dynamic(cols_);
    this->update_cycles(cols_ * 1);
}

// TODO Dram write
// todo negate should also be alu op
void SCAMP5RM::mov(AREG y, AREG x0) {
    // y = x0

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            this->dram->write_byte(row, col, NEWS, -x0_val);
            this->dram->write_byte(row, col, y, x0_val);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3);
}

// TODO cycles
void SCAMP5RM::res(AREG a) {
    // a = 0

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_byte(row, col, NEWS, 0);
            this->dram->write_byte(row, col, a, 0);
        }
    }
    dram->update_dynamic(cols_ * 8 * 2);
    this->update_cycles(cols_ * 8 * 2);
}

// TODO cycles
void SCAMP5RM::res(AREG a, AREG b) {
    // a = 0, b := 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_byte(row, col, NEWS, 0);
            this->dram->write_byte(row, col, a, 0);
            this->dram->write_byte(row, col, b, 0);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3);
}

// TODO ALU
void SCAMP5RM::add(AREG y, AREG x0, AREG x1) {
    // y = x0 + x1

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            int x1_val = this->dram->read_byte(row, col, x1);
            int sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
            this->dram->write_byte(row, col, NEWS, -sum);
            this->dram->write_byte(row, col, y, sum);
        }
    }

    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_);
}

// TODO ALU
void SCAMP5RM::add(AREG y, AREG x0, AREG x1, AREG x2) {
    // y = x0 + x1 + x2
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            int x1_val = this->dram->read_byte(row, col, x1);
            int x2_val = this->dram->read_byte(row, col, x2);
            int middle = this->alu->execute(x0_val, x1_val, ALU::ADD);
            int sum = this->alu->execute(middle, x2_val, ALU::ADD);
            this->dram->write_byte(row, col, NEWS, -sum);
            this->dram->write_byte(row, col, y, sum);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 5);
    this->update_cycles(cols_ * 8 * 5 + cols_ * 2);
}

// TODO ALU
void SCAMP5RM::sub(AREG y, AREG x0, AREG x1) {
    // y = x0 - x1

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            int x1_val = this->dram->read_byte(row, col, x1);
            int sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, -x0_val);
            this->dram->write_byte(row, col, y, sub);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_);

}

// TODO ALU
void SCAMP5RM::neg(AREG y, AREG x0) {
    // y = -x0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, 0);
            this->dram->write_byte(row, col, y, neg);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3 + cols_ );
}

// TODO ALU
// todo masking
void SCAMP5RM::abs(AREG y, AREG x0) {
    // y = |x0|

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);
            this->dram->write_byte(row, col, y, neg);
        }
    }

    this->where(x0);
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int news = this->dram->read_byte(row, col, NEWS);
            int neg = this->alu->execute(0, news, ALU::SUB);
            bool f = this->dram->read_bit(row, col, FLAG); //mask
            if (f) {
                this->dram->write_byte(row, col, y, neg);
            }
        }
    }
    this->all();
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ * 2);
}

// TODO ALU
void SCAMP5RM::div(AREG y0, AREG y1, AREG y2) {
    // y0 := y2/2; y1 := -y2/2, y2 := y2

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int y1_val = this->dram->read_byte(row, col, y1);
            int y2_val = this->dram->read_byte(row, col, y2);
            int add = this->alu->execute(y1_val, y2_val, ALU::ADD);
            int neg = this->alu->execute(0, add, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            int half_y2 = this->alu->execute(y2_val, 2, ALU::DIV);
            this->dram->write_byte(row, col, y0, half_y2);

            int neg_half_y2 = this->alu->execute(0, half_y2, ALU::SUB);
            this->dram->write_byte(row, col, y1, neg_half_y2);
        }
    }
    alu->update_dynamic(cols_ * 4);
    dram->update_dynamic(cols_ * 8 * 5);
    this->update_cycles(cols_ * 8 * 5 + cols_ * 4);
}

// TODO ALU
void SCAMP5RM::div(AREG y0, AREG y1, AREG y2, AREG x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0, y2 := x0

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int y1_val = this->dram->read_byte(row, col, y1);
            int x0_val = this->dram->read_byte(row, col, x0);
            this->dram->write_byte(row, col, y2, x0_val);

            int add = this->alu->execute(x0_val, y1_val, ALU::ADD);
            int neg = this->alu->execute(0, add, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            int half_x0 = this->alu->execute(x0_val, 2, ALU::DIV);
            this->dram->write_byte(row, col, y0, half_x0);

            int neg_half_x0 = this->alu->execute(0, half_x0, ALU::SUB);
            this->dram->write_byte(row, col, y1, neg_half_x0);
        }
    }
    alu->update_dynamic(cols_ * 4);
    dram->update_dynamic(cols_ * 8 * 6);
    this->update_cycles(cols_ * 8 * 6 + cols_ * 4);
}

// TODO ALU
void SCAMP5RM::diva(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0, y2 := -0.5*y0

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int y0_val = this->dram->read_byte(row, col, y0);
            int y1_val = this->dram->read_byte(row, col, y1);

            int add = this->alu->execute(y0_val, y1_val, ALU::ADD);
            int neg = this->alu->execute(0, add, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            int half_y0 = this->alu->execute(y0_val, 2, ALU::DIV);
            this->dram->write_byte(row, col, y0, half_y0);

            int neg_half_y0 = this->alu->execute(0, half_y0, ALU::SUB);
            this->dram->write_byte(row, col, y1, neg_half_y0);
            this->dram->write_byte(row, col, y2, neg_half_y0);
        }
    }
    alu->update_dynamic(cols_ * 4);
    dram->update_dynamic(cols_ * 8 * 6);
    this->update_cycles(cols_ * 8 * 6 + cols_ * 4);
}

// TODO ALU
void SCAMP5RM::divq(AREG y0, AREG x0) {
    // y0 := 0.5*x0 + error

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int x0_val = this->dram->read_byte(row, col, x0);

            int half_x0 = this->alu->execute(x0_val, 2, ALU::DIV);
            int neg = this->alu->execute(0, half_x0, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);
            this->dram->write_byte(row, col, y0, half_x0);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3 + cols_ * 2);
}

// TODO If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::movx(AREG y, AREG x0, news_t dir) {
    // y = x0_dir
    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int x0_val = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row, col, y, x0_val);
            this->dram->write_byte(row, col, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3 + cols_ );

    this->update_cycles(1);  // movement?
}

// If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::mov2x(AREG y, AREG x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir2 (note: this only works when FLAG is "all")

    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            switch (dir2) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_ && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            this->dram->write_byte(row, col, y, x0_val);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ );
    this->update_cycles(2);  // movement
}

// If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::addx(AREG y, AREG x0, AREG x1, news_t dir) {
    // y := x0_dir + x1_dir

    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int sum = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        int x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row, dram_row_select, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        int x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row, dram_row_select, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        int x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        int x1_val = this->dram->read_byte(dram_array_select, col, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        int x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        int x1_val = this->dram->read_byte(dram_array_select, col, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, sum, ALU::SUB);
            this->dram->write_byte(row, col, y, sum);
            this->dram->write_byte(row, col, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ * 2);
    this->update_cycles(1);  //movement
}

// If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::add2x(AREG y, AREG x0, AREG x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    PlaneParams p;
    get_dir_params(p, dir, dir2, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int x1_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    dram_array_select = row;
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row, dram_row_select, x1);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = row;
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row, dram_row_select, x1);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    dram_row_select = col;
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        x1_val = this->dram->read_byte(dram_array_select, col, x1);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = col;
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        x1_val = this->dram->read_byte(dram_array_select, col, x1);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
            int neg = this->alu->execute(0, sum, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            switch (dir2) {
                case east: {
                    dram_row_select = dram_row_select - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                        x1_val = this->dram->read_byte(dram_array_select, dram_row_select, x1);
                    }
                    break;
                }
                case west: {
                    dram_row_select = dram_row_select + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                        x1_val = this->dram->read_byte(dram_array_select, dram_row_select, x1);
                    }
                    break;
                };
                case north: {
                    dram_array_select = dram_array_select - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_ && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                        x1_val = this->dram->read_byte(dram_array_select, dram_row_select, x1);
                    }
                    break;
                };
                case south: {
                    dram_array_select = dram_array_select + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                        x1_val = this->dram->read_byte(dram_array_select, dram_row_select, x1);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            this->dram->write_byte(row, col, y, this->alu->execute(x0_val, x1_val, ALU::ADD));
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 8 * 6);
    this->update_cycles(cols_ * 8 * 6 + cols_ * 3);
    this->update_cycles(2);  // movement
}

// If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::subx(AREG y, AREG x0, news_t dir, AREG x1) {
    // y := x0_dir - x1
    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int sub = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        int x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row, col, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        int x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row, col, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        int x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        int x1_val = this->dram->read_byte(row, col, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        int x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        int x1_val = this->dram->read_byte(row, col, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, sub, ALU::SUB);
            this->dram->write_byte(row, col, y, sub);
            this->dram->write_byte(row, col, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ * 2);
    this->update_cycles(1);  // movement
}

// If east or west, then same row, same dram array. if north then north array, and same for south
void SCAMP5RM::sub2x(AREG y, AREG x0, news_t dir, news_t dir2, AREG x1) {
    // y := x0_dir_dir2 - x1
    PlaneParams p;
    get_dir_params(p, dir, dir2, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int x1_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    dram_array_select = row;
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row, col, x1);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = row;
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row, col, x1);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    dram_row_select = col;
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        x1_val = this->dram->read_byte(row, col, x1);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    dram_row_select = col;
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col, x0);
                        x1_val = this->dram->read_byte(row, col, x1);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
            int neg = this->alu->execute(0, sub, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg);

            switch (dir2) {
                case east: {
                    dram_row_select = dram_row_select - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    dram_row_select = dram_row_select + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_array_select < this->rows_ && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    dram_array_select = dram_array_select - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_ && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case south: {
                    dram_array_select = dram_array_select + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0 && dram_row_select >= 0 && dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(dram_array_select, dram_row_select, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            this->dram->write_byte(row, col, y, this->alu->execute(x0_val, x1_val, ALU::SUB));
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 8 * 5);
    this->update_cycles(cols_ * 8 * 5 + cols_ * 3);
    this->update_cycles(2);  // movement
}

// TODO ALU
void SCAMP5RM::OR(DREG d, DREG d0, DREG d1) {
    // d := d0 OR d1
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int res = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            this->dram->write_bit(row, col, d, res);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3 + cols_);
    this->update_cycles(4); // 2 reads, 1 op, 1 write
}

// TODO ALU
void SCAMP5RM::OR(DREG d, DREG d0, DREG d1, DREG d2) {
    // d := d0 OR d1 OR d2
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int res = this->alu->execute(d0_d1, dram->read_bit(row, col, d2), ALU::OR);
            this->dram->write_bit(row, col, d, res);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4 + cols_ * 2);
    this->update_cycles(5);  // 3 reads, 1 op, 1 write
}

// TODO ALU
void SCAMP5RM::OR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {
    // d := d0 OR d1 OR d2 OR d3
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int d0_d1_d2 = this->alu->execute(d0_d1, dram->read_bit(row, col, d2), ALU::OR);
            int res = this->alu->execute(d0_d1_d2, dram->read_bit(row, col, d3), ALU::OR);
            this->dram->write_bit(row, col, d, res);
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 1 * 6);
    this->update_cycles(cols_ * 1 * 6 + cols_ * 3);
    this->update_cycles(6);  // 4 reads, 1 op, 1 write
}

// TODO ALU
void SCAMP5RM::NOT(DREG d, DREG d0) {
    // d := NOT d0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int negated = this->alu->execute(dram->read_bit(row, col, d0), 1, ALU::XOR);
            this->dram->write_bit(row, col, d, negated);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 1 * 2);
    this->update_cycles(cols_ * 1 * 2 + cols_);
    this->update_cycles(3); // 1 read, 1 op, 1 write
}

// TODO ALU
void SCAMP5RM::NOR(DREG d, DREG d0, DREG d1) {
    // d := NOT(d0 OR d1)
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int negated = this->alu->execute(d0_d1, 1, ALU::XOR);
            this->dram->write_bit(row, col, d, negated);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3 + cols_ * 2);
    this->update_cycles(5);  // 2 reads, 2 op, 1 write
}

// TODO ALU
void SCAMP5RM::NOR(DREG d, DREG d0, DREG d1, DREG d2) {
    // d := NOT(d0 OR d1 OR d2)
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int res = this->alu->execute(d0_d1, dram->read_bit(row, col, d2), ALU::OR);
            int negated = this->alu->execute(res, 1, ALU::XOR);
            this->dram->write_bit(row, col, d, negated);
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4 + cols_ * 3);
    this->update_cycles(6);  // 3 reads, 2 op, 1 write
}

// TODO ALU
void SCAMP5RM::NOR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int d0_d1_d2 = this->alu->execute(d0_d1, dram->read_bit(row, col, d2), ALU::OR);
            int res = this->alu->execute(d0_d1_d2, dram->read_bit(row, col, d3), ALU::OR);
            int negated = this->alu->execute(res, 1, ALU::XOR);
            this->dram->write_bit(row, col, d, negated);
        }
    }
    alu->update_dynamic(cols_ * 4);
    dram->update_dynamic(cols_ * 1 * 5);
    this->update_cycles(cols_ * 1 * 5 + cols_ * 4);
    this->update_cycles(7); // 4 reads, 2 op, 1 write
}

// TODO ALU
void SCAMP5RM::NOT(DREG Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
    this->update_cycles(4); // 2 reads, 1 op, 1 write
}

// TODO ALU
void SCAMP5RM::OR(DREG Rl, DREG Rx) {
    // Rl := Rl OR Rx
    this->OR(Rl, Rl, Rx);
}

// TODO ALU
void SCAMP5RM::NOR(DREG Rl, DREG Rx) {
    // Rl := Rl NOR Rx
    this->NOR(Rl, Rl, Rx);
}

// TODO ALU
void SCAMP5RM::AND(DREG Ra, DREG Rx, DREG Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(R0);
    this->NOT(R12, Rx);
    this->NOT(R0, Ry);
    this->NOR(Ra, R0, R12);
}

// TODO ALU
void SCAMP5RM::NAND(DREG Ra, DREG Rx, DREG Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(R0);
    this->NOT(R12, Rx);
    this->NOT(R0, Ry);
    this->OR(Ra, R0, R12);
}

// TODO ALU
void SCAMP5RM::ANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(R0, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, R0, Rb);
}

// TODO ALU
void SCAMP5RM::NANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(R0, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, R0, Rb);
}

// TODO ALU
void SCAMP5RM::IMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx IMP Ry (logical implication)
    //    Truth Table:
    //    Rx  Ry    Rl
    //    0   0     1
    //    0   1     0
    //    1   0     1
    //    1   1     1
    this->NOT(R0, Ry);
    this->OR(R1, Rx, R0);
}

// TODO ALU
void SCAMP5RM::NIMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(R0, Ry);
    this->NOR(R1, Rx, R0);
}

// TODO ALU
void SCAMP5RM::XOR(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(R0, Ry);
    this->NOR(Rl, Rx, R0);
    this->NOT(R0, Rx);
    this->NOR(Rx, Ry, R0);
    this->OR(Rl, Rx);
}

// TODO DRAM write - masking
void SCAMP5RM::WHERE(DREG d) {
    // FLAG := d.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, FLAG, this->dram->read_bit(row, col, d));
        }
    }
    dram->update_dynamic(cols_ * 1 * 2);
    this->update_cycles(cols_ * 1 * 2);
}

// TODO DRAM write
void SCAMP5RM::WHERE(DREG d0, DREG d1) {
    // FLAG := d0 OR d1.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            this->dram->write_bit(row, col, FLAG, d0_d1);
        }
    }
    alu->update_dynamic(cols_ * 1);
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3 + cols_);
}

// TODO DRAM write
void SCAMP5RM::WHERE(DREG d0, DREG d1, DREG d2) {
    // FLAG := d0 OR d1 OR d2.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_d1 = this->alu->execute(dram->read_bit(row, col, d0), dram->read_bit(row, col, d1), ALU::OR);
            int res = this->alu->execute(d0_d1, dram->read_bit(row, col, d2), ALU::OR);
            this->dram->write_bit(row, col, FLAG, res);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4 + cols_ * 2);
}

// TODO DRAM write
void SCAMP5RM::ALL() {
    // FLAG := 1, same as all.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, FLAG, 1);
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1 * 3 + cols_);
}

// TODO DRAM write
void SCAMP5RM::SET(DREG d0) {
    // d0 := 1
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 1);
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

// TODO DRAM write
void SCAMP5RM::SET(DREG d0, DREG d1) {
    // d0, d1 := 1
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 1);
            this->dram->write_bit(row, col, d1, 1);
        }
    }
    dram->update_dynamic(cols_ * 1 * 2);
    this->update_cycles(cols_ * 1 * 2);
}

// TODO DRAM write
void SCAMP5RM::SET(DREG d0, DREG d1, DREG d2) {
    // 	d0, d1, d2 := 1
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 1);
            this->dram->write_bit(row, col, d1, 1);
            this->dram->write_bit(row, col, d2, 1);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}

// TODO DRAM write
void SCAMP5RM::SET(DREG d0, DREG d1, DREG d2, DREG d3) {
    // d0, d1, d2, d3 := 1
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 1);
            this->dram->write_bit(row, col, d1, 1);
            this->dram->write_bit(row, col, d2, 1);
            this->dram->write_bit(row, col, d3, 1);
        }
    }
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4);
}

// TODO DRAM write
void SCAMP5RM::CLR(DREG d0) {
    // d0 := 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 0);
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

// TODO DRAM write
void SCAMP5RM::CLR(DREG d0, DREG d1) {
    // d0, d1 := 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 0);
            this->dram->write_bit(row, col, d1, 0);
        }
    }
    dram->update_dynamic(cols_ * 1 * 2);
    this->update_cycles(cols_ * 1 * 2);
}

// TODO DRAM write
void SCAMP5RM::CLR(DREG d0, DREG d1, DREG d2) {
    // d0, d1, d2 := 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 0);
            this->dram->write_bit(row, col, d1, 0);
            this->dram->write_bit(row, col, d2, 0);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}

// TODO DRAM write
void SCAMP5RM::CLR(DREG d0, DREG d1, DREG d2, DREG d3) {
    // 	d0, d1, d2, d3 := 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d0, 0);
            this->dram->write_bit(row, col, d1, 0);
            this->dram->write_bit(row, col, d2, 0);
            this->dram->write_bit(row, col, d3, 0);
        }
    }
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4);
}

// TODO DRAM write
void SCAMP5RM::MOV(DREG d, DREG d0) {
    // d := d0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            this->dram->write_bit(row, col, d, this->dram->read_bit(row, col, d0));
        }
    }
    dram->update_dynamic(cols_ * 1 * 2);
    this->update_cycles(cols_ * 1 * 2);
}

// TODO ALU
void SCAMP5RM::MUX(DREG Rl, DREG Rx, DREG Ry, DREG Rz) {
    // Rl := Ry IF Rx = 1, Rl := Rz IF Rx = 0.
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            bool rx_v = dram->read_bit(row, col, Rx);
            int negated = this->alu->execute(rx_v, 1, ALU::XOR);
            int a1 = this->alu->execute(this->dram->read_bit(row, col, Ry), negated, ALU::AND);
            int a2 = this->alu->execute(this->dram->read_bit(row, col, Rz), rx_v, ALU::AND);
            int res = this->alu->execute(a1, a2, ALU::OR);
            this->dram->write_bit(row, col, Rl, res);
        }
    }
    alu->update_dynamic(cols_ * 4);
    dram->update_dynamic(cols_ * 1 * 4);
    this->update_cycles(cols_ * 1 * 4 + cols_ * 4);
}

void SCAMP5RM::CLR_IF(DREG Rl, DREG Rx) {
    // Rl := 0 IF Rx = 1, Rl := Rl IF Rx = 0
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            bool rl_v = dram->read_bit(row, col, Rl);
            bool rx_v = dram->read_bit(row, col, Rx);
            int negated = this->alu->execute(rl_v, 1, ALU::XOR);

            int res = this->alu->execute(negated, rx_v, ALU::OR);
            int nor = this->alu->execute(res, 1, ALU::XOR);
            this->dram->write_bit(row, col, Rl, nor);
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3 + cols_ * 3);
}

void SCAMP5RM::REFRESH(DREG Rl) {
    // 	refresh a DREG to prevent decay after a long time (e.g. > 1.5 seconds)
    // without any operations
}

// todo maybe using the AND and OR method
void SCAMP5RM::DNEWS0(DREG d, DREG d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 0 from the edge

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_val = 0;

            if (this->dram->read_bit(row, col, R1)) {
                // south
                int dram_array_select = this->rows_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_array_select >= 0) {
                    d0_val = this->dram->read_bit(dram_array_select, col, d0);
                }
            } else if (this->dram->read_bit(row, col, R2)) {
                // west
                int dram_row_select = this->cols_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_row_select < this->cols_) {
                    d0_val = this->dram->read_bit(row, dram_row_select, d0);
                }
            }  else if (this->dram->read_bit(row, col, R3)) {
                // north
                int dram_array_select = this->rows_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_array_select < this->rows_) {
                    d0_val = this->dram->read_bit(dram_array_select, col, d0);
                }
            } else if (this->dram->read_bit(row, col, R4)) {
                // east
                int dram_row_select = this->cols_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_row_select >= 0) {
                    d0_val = this->dram->read_bit(row, dram_row_select, d0);
                }
            }

            this->dram->write_bit(row, col, d, d0_val);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}

void SCAMP5RM::DNEWS1(DREG d, DREG d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 1 from the edge
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_val = 1;

            if (this->dram->read_bit(row, col, R1)) {
                // south
                int dram_array_select = this->rows_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_array_select >= 0) {
                    d0_val = this->dram->read_bit(dram_array_select, col, d0);
                }
            } else if (this->dram->read_bit(row, col, R2)) {
                // west
                int dram_row_select = this->cols_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_row_select < this->cols_) {
                    d0_val = this->dram->read_bit(row, dram_row_select, d0);
                }
            }  else if (this->dram->read_bit(row, col, R3)) {
                // north
                int dram_array_select = this->rows_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_array_select < this->rows_) {
                    d0_val = this->dram->read_bit(dram_array_select, col, d0);
                }
            } else if (this->dram->read_bit(row, col, R4)) {
                // east
                int dram_row_select = this->cols_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_row_select >= 0) {
                    d0_val = this->dram->read_bit(row, dram_row_select, d0);
                }
            }

            this->dram->write_bit(row, col, d, d0_val);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}

void SCAMP5RM::DNEWS(DREG Ra, DREG Rx, int dir, bool boundary) {
    // digital neighbour OR, Ra := Rx_dir1 OR Rx_dir2 ...; (also modify R1 R2 R3
    // R4).
    this->CLR(R1, R2, R3, R4);
    // set multiple DREG& (upto 4) can be done via one icw
    if(dir & south) {
        this->SET(R1);
    }
    if(dir & west) {
        this->SET(R2);
    }
    if(dir & north) {
        this->SET(R3);
    }
    if(dir & east) {
        this->SET(R4);
    }

    if(boundary) {
        this->DNEWS1(Ra, Rx);
    } else {
        this->DNEWS0(Ra, Rx);
    }
}

void SCAMP5RM::scamp5_in(AREG areg, int16_t value, AREG temp) {
    // load an analog value to the AREG
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, IN, value);
            this->dram->write_byte(row, col, temp, -value);
            this->dram->write_byte(row, col, areg, value);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3);
}

void SCAMP5RM::scamp5_in(AREG areg, int16_t value) {
    // load an analog value to the AREG
    scamp5_in(areg, value, NEWS);
}

void SCAMP5RM::scamp5_load_in(AREG areg, int8_t value, AREG temp) {
    // load a analog value to the AREG plane
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, IN, value);
            this->dram->write_byte(row, col, temp, -value);
            this->dram->write_byte(row, col, areg, value);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3);
}

void SCAMP5RM::scamp5_load_in(AREG areg, int8_t value) {
    // load a analog value to the AREG plane without error&noise correction
    scamp5_load_in(areg, value, NEWS);
}

void SCAMP5RM::scamp5_load_in(int8_t value) {
    // load a analog value to IN
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, IN, value);
        }
    }
    dram->update_dynamic(cols_ * 8 );
    this->update_cycles(cols_ * 8);
}

void SCAMP5RM::scamp5_load_dac(AREG areg, uint16_t value, AREG temp) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	temporary kernel register to be used in the function
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, IN, value);
            this->dram->write_byte(row, col, temp, -value);
            this->dram->write_byte(row, col, areg, value);
        }
    }
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3);
}

void SCAMP5RM::scamp5_load_dac(AREG areg, uint16_t value) {
    // load an analog value to the AREG plane using a raw DAC value
    // areg	target AREG
    // value a 12-bit DAC value to use (in the range of [0,4095])
    // temp	= NEWS
    scamp5_load_dac(areg, value, NEWS);
}

void SCAMP5RM::scamp5_load_dac(uint16_t value) {
    // load an analog value to IN using a raw DAC value
    // TODO What is with the range of values here. Why can some registers hold a
    // much larger range?
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, IN, value);
        }
    }
    dram->update_dynamic(cols_ * 8);
    this->update_cycles(cols_ * 8);
}

uint8_t SCAMP5RM::scamp5_read_areg(AREG areg, uint8_t r, uint8_t c) {
    // read a single pixel
    // TODO check that the value is properly mapped to uint8_5
    return this->dram->read_byte(r, c, areg);
    dram->update_dynamic(8);
    this->update_cycles(8);
}


int SCAMP5RM::scamp5_global_or(DREG dreg, uint8_t r, uint8_t c, uint8_t rx,
                             uint8_t cx) {
    // get OR result of all pixels in a DREG plane. 0 if all pixel is 0,
    // non-zero otherwise. The default mask pattern parameters gives the whole
    // image. scamp5_load_pattern can be used to work out the correct parameter
    // for a desired mask pattern,
    // TODO abstraction
    unsigned int r_mask = ((~rx) & (this->rows_ - 1));
    unsigned int c_mask = ((~cx) & (this->cols_ - 1));

    unsigned int r_f = r & r_mask;
    unsigned int c_f = c & c_mask;

    uint8_t val = 0;

    for(int row_index = 0; row_index < this->rows_; row_index++) {
        for(int col_index = 0; col_index < this->cols_; col_index++) {
            if(((row_index & r_mask) == r_f) && ((col_index & c_mask) == c_f)) {
                val |= this->dram->read_bit(row_index, col_index, dreg);
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
    return val;
}

int SCAMP5RM::scamp5_global_count(DREG dreg, AREG temp, int options) {
    // get an estimation of the number of '1's in a DREG plane
    int count = 0;
    for(int row_index = 0; row_index < this->cols_; row_index++) {
        for(int col_index = 0; col_index < this->rows_; col_index++) {
            count += this->dram->read_bit(row_index, col_index, dreg);
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
    return count;
}


void SCAMP5RM::scamp5_load_point(DREG dr, uint8_t r, uint8_t c) {
    // set a single pixel on a DREG image to 1, the rest to 0
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_bit(row, col, dr, row == r && col == c ? 1 : 0);
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

void SCAMP5RM::scamp5_load_rect(DREG dr, uint8_t r0, uint8_t c0, uint8_t r1,
                              uint8_t c1) {
    // set a rectangle area on a DREG image to 1, the rest to 0
    // r0	pixel row index of the top right corner
    // c0	pixel column index of the top right corner
    // r1	pixel row index of the bottom left corner
    // c1	pixel column index of the bottom left corner


    for (int row = r0; row < r1; ++row) {
        for (int col = c0; col < c1; ++col) {
            this->dram->write_bit(row, col, dr, 0);
        }
    }

    for (int row = r0; row < r1; ++row) {
        for (int col = c0; col < c1; ++col) {
            this->dram->write_bit(row, col, dr, 1);
        }
    }
    dram->update_dynamic(cols_ * 2);
    this->update_cycles(cols_ * 2);
}

void SCAMP5RM::scamp5_load_pattern(DREG dr, uint8_t r, uint8_t c, uint8_t rx,
                                 uint8_t cx) {
    // set those pixels with matching address to 1, the rest to 0
    // To mask out certain bits in the column/row address allow a match to occur
    // periodically. For example, set mask to 192 (11000000b) makes the four
    // following address match the value 3 (00000011b): 3(00000011b),
    // 67(01000011b), 131(10000011b) and 195(11000011b).
    // TODO abstraction

    unsigned int r_mask = ((~rx) * (this->rows_ - 1));
    unsigned int c_mask = ((~cx) * (this->cols_ - 1));

    unsigned int r_f = r * r_mask;
    unsigned int c_f = c * c_mask;

    for(int row_index = 0; row_index < this->cols_; row_index++) {
        for(int col_index = 0; col_index < this->rows_; col_index++) {
            if(((row_index * r_mask) == r_f) && ((col_index * c_mask) == c_f)) {
                this->dram->write_bit(row_index, col_index, dr, 1);
            } else {
                this->dram->write_bit(row_index, col_index, dr, 0);
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

void SCAMP5RM::scamp5_select_point(uint8_t r, uint8_t c) {
    // same as scamp5_load_point, but put the point in SELECT
    scamp5_load_point(SELECT, r, c);
}

void SCAMP5RM::scamp5_select_rect(uint8_t r0, uint8_t c0, uint8_t r1,
                                uint8_t c1) {
    // same as scamp5_load_rect, but put the rectangle in RECT (also modify
    // SELECT)
    // TODO why is SELECT modified?
    scamp5_load_rect(RECT, r0, c0, r1, c1);
}

void SCAMP5RM::scamp5_select_pattern(uint8_t r, uint8_t c, uint8_t rx,
                                   uint8_t cx) {
    // same as scamp5_load_pattern, but put the pattern in SELECT
    // TODO check
    scamp5_load_pattern(SELECT, r, c, rx, cx);
}

void SCAMP5RM::scamp5_select_col(uint8_t c) {
    // select column
    for(int row_index = 0; row_index < this->cols_; row_index++) {
        for(int col_index = 0; col_index < this->rows_; col_index++) {
            if(col_index == c) {
                this->dram->write_bit(row_index, col_index, SELECT, 1);
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

void SCAMP5RM::scamp5_select_row(uint8_t r) {
    // select row
    for(int row_index = 0; row_index < this->cols_; row_index++) {
        for(int col_index = 0; col_index < this->rows_; col_index++) {
            if(row_index == r) {
                this->dram->write_bit(row_index, col_index, SELECT, 1);
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

void SCAMP5RM::scamp5_select_colx(uint8_t cx) {
    // select column mask
}

void SCAMP5RM::scamp5_select_rowx(uint8_t rx) {
    // select row mask
}

void SCAMP5RM::scamp5_draw_begin(DREG dr) {
    // targeting a DREG image to a series of drawing operations
    scratch = dr;
}

void SCAMP5RM::scamp5_draw_end() {
    // end the drawing operations
    scratch = R0;
}

void SCAMP5RM::scamp5_draw_pixel(uint8_t r, uint8_t c) {
    // draw a point, wrap around if it's outside the border
    this->dram->write_bit(r % this->rows_, c % this->cols_, scratch, 1);
    dram->update_dynamic(1);
    this->update_cycles(1);
}

bool SCAMP5RM::scamp5_draw_point(int r, int c) {
    // draw a point when its position is within the image
    // returns whether the point is inside the image and drawn
    if(r >= this->rows_ || c >= this->cols_) {
        return false;
    }
    this->dram->write_bit(r, c, scratch, 1);
    dram->update_dynamic(1);
    this->update_cycles(1);
    return true;
}

//void SCAMP5RM::scamp5_draw_rect(uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1) {
//    // draw a filled rectangle
//    // r0 pixel row index of the top right corner
//    // c0 pixel column index of the top right corner
//    // r1 pixel row index of the bottom left corner
//    // c1 pixel column index of the bottom left corner
//    // FIXME issue with coordinates
//    int width = c1 - c0;
//    int height = r1 - r0;
//
//    scratch->clear();
//    scratch->read()(cv::Rect(c0, r0, width, height)).setTo(1);
//}



void SCAMP5RM::scamp5_draw_negate() {
    // do a binary inversion of the DREG image.
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_bit(row, col, scratch, 1- this->dram->read_bit(row, col, scratch));
        }
    }
    dram->update_dynamic(cols_ * 2);
    this->update_cycles(cols_ * 2);
}

// Image Readout

void SCAMP5RM::scamp5_scan_areg(AREG areg, uint8_t *buffer, uint8_t r0,
                              uint8_t c0, uint8_t r1, uint8_t c1, uint8_t rs,
                              uint8_t cs) {
    // scan a customized grid of pixels in an AREG image
    // Note, the result image is stored in column-major format, starting from
    // top right. i.e. "buffer[3]" is the pixel the on 1st column right, 4th row
    // down. This applies to all "scamp5_scan_areg_*" series functions.
    PlaneParams p;
    get_fixed_params(p, this->origin_, r0, c0, r1, c1, rs, cs);

    int buf_index = 0;
    for(int col = p.col_start; p.col_op(col, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row, p.row_end); row += p.row_step) {
            buffer[buf_index++] = this->dram->read_byte(row, col, areg);
        }
    }
    dram->update_dynamic(cols_ * 8);
    this->update_cycles(cols_ * 8);
}

void SCAMP5RM::scamp5_scan_areg_8x8(AREG areg, uint8_t *result8x8) {
    // scan a 8x8 grid of pixels in an AREG image
    // This function is slightly faster and more accurate than scamp5_scan_areg.
    // TODO currently assuming this takes beginning of each cell in 8x8 grid.
    // Might be center instead
    int buf_index = 0;
    int cs = this->cols_ / 8;
    int rs = this->rows_ / 8;
    for(int col = 0; col < this->cols_; col += cs) {
        for(int row = 0; row < this->rows_; row += rs) {
            result8x8[buf_index++] = this->dram->read_byte(row, col, areg);
        }
    }
    dram->update_dynamic(cols_ * 8);
    this->update_cycles(cols_ * 8);
}

void SCAMP5RM::scamp5_scan_dreg(DREG dreg, uint8_t *mem, uint8_t r0,
                              uint8_t r1) {
    // scan DREG image, store the result in a buffer
    // mem - pointer to a buffer
    // r0 - first row index
    // r1 - last row index
    // The size of the buffer need to be a least 32 times the number of rows to
    // scan. Thus, a full DREG image requires a buffer of 8192 bytes.
    // TODO check impl
    int buf_index = 0;
    for(uint32_t row_index = r0; row_index <= r1; row_index++) {
        // Read 8 values at a time to make up a byte
        for(int col_index = 0; col_index < this->cols_; col_index += 8) {
            this->dram->read_byte(row_index, col_index, dreg);
            uint8_t b0 = this->dram->read_bit(row_index, col_index, dreg);
            uint8_t b1 = this->dram->read_bit(row_index, col_index + 1, dreg);
            uint8_t b2 = this->dram->read_bit(row_index, col_index + 2, dreg);
            uint8_t b3 = this->dram->read_bit(row_index, col_index + 3, dreg);
            uint8_t b4 = this->dram->read_bit(row_index, col_index + 4, dreg);
            uint8_t b5 = this->dram->read_bit(row_index, col_index + 5, dreg);
            uint8_t b6 = this->dram->read_bit(row_index, col_index + 6, dreg);
            uint8_t b7 = this->dram->read_bit(row_index, col_index + 7, dreg);
            uint8_t value = (b0 << 7) | (b1 << 6) | (b2 << 5) | (b3 << 4) |
                            (b4 << 3) | (b5 << 2) | (b6 << 1) | (b7 << 0);
            mem[buf_index++] = value;
        }
    }
    dram->update_dynamic(cols_ * 8);
    this->update_cycles(cols_ * 8);
}

void SCAMP5RM::scamp5_scan_events(DREG dreg, uint8_t *mem, uint16_t max_num,
                                uint8_t h_dir, uint8_t v_dir) {
    // scan the coordinates of all '1's in a DREG image and store the result in
    // a buffer mem - pointer to a buffer of (max_num*2) bytes max_num - maximum
    // number of events h_dir - horizontal order of scanning, '0' means from
    // column 0 to column 255 v_dir - vertical order of scanning, '0' means from
    // row 0 to row 255 Up-to 4000 events scan be scanned. Note, the execution
    // time of this function is proportional to max_num. In the result, the
    // first occurrence of (0,0) indicates the end of effective events. writes
    // to mem as (col, row)
    // TODO Row/Col confusion. Make consistent
    // FIXME with proper setting of (0, 0)
    // TODO does not take scanning direction into account. Rewrite
    int buf_index = 0;
    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            if(buf_index == 2 * max_num) return;
            bool bit = this->dram->read_bit(row, col, dreg);
            if (bit) {
                mem[buf_index++] = col;
                mem[buf_index++] = row;
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);
}

void SCAMP5RM::scamp5_scan_events(DREG dreg, uint8_t *buffer, uint16_t max_num,
                                uint8_t r0, uint8_t c0, uint8_t r1, uint8_t c1,
                                uint8_t rs, uint8_t cs) {
    // assuming 0,0 in top left

    int buf_index = 0;
    for (int row = r0; row < r1; row+=rs) {
        for (int col = c0; col < c1; col+=cs) {
            if(buf_index == 2 * max_num) return;
            bool bit = this->dram->read_bit(row, col, dreg);
            if (bit) {
                buffer[buf_index++] = col;
                buffer[buf_index++] = row;
            }
        }
    }
    dram->update_dynamic(cols_ * 1);
    this->update_cycles(cols_ * 1);

}
// Simulator specific

void SCAMP5RM::display() {
    cv::Mat val_a = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_b = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_c = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_d = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_r5 = cv::Mat::zeros(cv::Size(rows_, cols_), CV_8U);

    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            val_a.at<int16_t>(row, col) = this->dram->read_byte(row, col, A);
            val_b.at<int16_t>(row, col) = this->dram->read_byte(row, col, B);
            val_c.at<int16_t>(row, col) = this->dram->read_byte(row, col, C);
            val_d.at<int16_t>(row, col) = this->dram->read_byte(row, col, D);
            val_r5.at<uint8_t>(row, col) = this->dram->read_bit(row, col, R5);
        }
    }
    std::shared_ptr<AnalogueRegister> ar = std::make_shared<AnalogueRegister>(val_a);
    ar->name_ = "A";

    std::shared_ptr<AnalogueRegister> br = std::make_shared<AnalogueRegister>(val_b);
    br->name_ = "B";

    std::shared_ptr<AnalogueRegister> cr = std::make_shared<AnalogueRegister>(val_c);
    cr->name_ = "C";

    std::shared_ptr<AnalogueRegister> dr = std::make_shared<AnalogueRegister>(val_d);
    dr->name_ = "D";

    std::shared_ptr<DigitalRegister> r5r = std::make_shared<DigitalRegister>(val_r5);
    r5r->name_ = "R5";
    UI::get_instance().display_reg(ar);
    UI::get_instance().display_reg(br);
    UI::get_instance().display_reg(cr);
    UI::get_instance().display_reg(dr);
    UI::get_instance().display_reg(r5r);
}

//todo remove
void SCAMP5RM::print_stats() {
    // TODO move
#ifdef TRACK_STATISTICS
    this->update_static(); //move
    Architecture::print_stats(rows_, cols_);
//    json j;
//    j["Total number of cycles"] = counter->get_cycles();
//    j["Equivalent in seconds"] = counter->to_seconds(config_.clock_rate_);
//
//    // this.print_stats(counter);
//    this->write_stats(*counter, j);
//    std::cout << std::setw(2) << j << std::endl;
//    std::ofstream file_out;
//    std::cout << std::filesystem::current_path().string() << std::endl;
//    file_out.open(std::filesystem::current_path().string() + "/output.json");
//    file_out << std::setw(2) << j;
//    file_out.close();
#endif
#ifndef TRACK_STATISTICS
    std::cerr << "Simulator has not been compiled with statistic tracking support. Recompile with -DTRACK_STATISTICS=ON" << std::endl;
#endif
}

rttr::variant SCAMP5RM::config_converter(json& j) {
    return Parser::get_instance().create_instance("Config", j);
}

void SCAMP5RM::set_rows(int rows) {
    this->rows_ = rows;
}

void SCAMP5RM::set_cols(int cols) {
    this->cols_ = cols;
}

void SCAMP5RM::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void SCAMP5RM::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void SCAMP5RM::set_origin(Origin origin) {
    this->origin_ = origin;
}

void SCAMP5RM::set_config(std::shared_ptr<Config> config) {
    this->config_ = std::move(config);
}

void SCAMP5RM::set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components) {
    this->components_ = std::move(components);
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::enumeration<AREG>("AREG") (
        value("PIX", AREG::PIX),
        value("IN", AREG::IN),
        value("NEWS", AREG::NEWS),
        value("A", AREG::A),
        value("B", AREG::B),
        value("C", AREG::C),
        value("D", AREG::D),
        value("E", AREG::E),
        value("F", AREG::F)
    );

    registration::enumeration<DREG>("DREG") (
        value("FLAG", DREG::FLAG),
        value("SELECT", DREG::SELECT),
        value("RECT", DREG::RECT),
        value("R1", DREG::R1),
        value("R2", DREG::R2),
        value("R3", DREG::R3),
        value("R4", DREG::R4),
        value("R5", DREG::R5),
        value("R6", DREG::R6),
        value("R7", DREG::R7),
        value("R8", DREG::R8),
        value("R9", DREG::R9),
        value("R10", DREG::R10),
        value("R11", DREG::R11),
        value("R12", DREG::R12),
        value("R0", DREG::R0)
    );

    registration::class_<SCAMP5RM>("SCAMP5RM")
        .constructor()
        .method("init", &SCAMP5RM::init)
        .method("display", &SCAMP5RM::display)
        .method("config_converter", &SCAMP5RM::config_converter)
        .method("set_rows", &SCAMP5RM::set_rows)
        .method("set_cols", &SCAMP5RM::set_cols)
        .method("set_row_stride", &SCAMP5RM::set_row_stride)
        .method("set_col_stride", &SCAMP5RM::set_col_stride)
        .method("set_origin", &SCAMP5RM::set_origin)
        .method("set_config", &SCAMP5RM::set_config)
        .method("set_components", &SCAMP5RM::set_components)
        .method("nop", &SCAMP5RM::nop)
        .method("rpix", &SCAMP5RM::rpix)
        .method("get_image", select_overload<void(AREG)> (&SCAMP5RM::get_image))
        .method("get_image", select_overload<void(AREG, AREG)>(&SCAMP5RM::get_image))
        .method("where", select_overload<void(AREG)>(&SCAMP5RM::where))
        .method("where", select_overload<void(AREG, AREG)>(&SCAMP5RM::where))
        .method("where", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RM::where))
        .method("all", &SCAMP5RM::all)
        .method("mov", &SCAMP5RM::mov)
        .method("res", select_overload<void(AREG)>(&SCAMP5RM::res))
        .method("res", select_overload<void(AREG, AREG)>(&SCAMP5RM::res))
        .method("add", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RM::add))
        .method("add", select_overload<void(AREG, AREG, AREG, AREG)>(&SCAMP5RM::add))
        .method("sub", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RM::sub))
        .method("neg", &SCAMP5RM::neg)
        .method("abs", &SCAMP5RM::abs)
        .method("div", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RM::div))
        .method("div", select_overload<void(AREG, AREG, AREG, AREG)>(&SCAMP5RM::div))
        .method("diva", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RM::diva))
        .method("divq", select_overload<void(AREG, AREG)>(&SCAMP5RM::divq))
        .method("movx", &SCAMP5RM::movx)
        .method("mov2x", &SCAMP5RM::mov2x)
        .method("addx", &SCAMP5RM::addx)
        .method("add2x", &SCAMP5RM::add2x)
        .method("subx", &SCAMP5RM::subx)
        .method("sub2x", &SCAMP5RM::sub2x)
        .method("OR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RM::OR))
        .method("OR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RM::OR))
        .method("OR", select_overload<void(DREG, DREG, DREG, DREG, DREG)>(&SCAMP5RM::OR))
        .method("NOT", select_overload<void(DREG, DREG)>(&SCAMP5RM::NOT))
        .method("NOR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RM::NOR))
        .method("NOR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RM::NOR))
        .method("NOR", select_overload<void(DREG, DREG, DREG, DREG, DREG)>(&SCAMP5RM::NOR))
        .method("NOR", select_overload<void(DREG)>(&SCAMP5RM::NOT))
        .method("OR", select_overload<void(DREG, DREG)>(&SCAMP5RM::OR))
        .method("NOR", select_overload<void(DREG, DREG)>(&SCAMP5RM::NOR))
        .method("AND", &SCAMP5RM::AND)
        .method("NAND", &SCAMP5RM::NAND)
        .method("ANDX", &SCAMP5RM::ANDX)
        .method("NANDX", &SCAMP5RM::NANDX)
        .method("IMP", &SCAMP5RM::IMP)
        .method("NIMP", &SCAMP5RM::NIMP)
        .method("XOR", &SCAMP5RM::XOR)
        .method("WHERE", select_overload<void(DREG)>(&SCAMP5RM::WHERE))
        .method("WHERE", select_overload<void(DREG, DREG)>(&SCAMP5RM::WHERE))
        .method("WHERE", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RM::WHERE))
        .method("ALL", &SCAMP5RM::ALL)
        .method("SET", select_overload<void(DREG)>(&SCAMP5RM::SET))
        .method("SET", select_overload<void(DREG, DREG)>(&SCAMP5RM::SET))
        .method("SET", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RM::SET))
        .method("SET", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RM::SET))
        .method("CLR", select_overload<void(DREG)>(&SCAMP5RM::CLR))
        .method("CLR", select_overload<void(DREG, DREG)>(&SCAMP5RM::CLR))
        .method("CLR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RM::CLR))
        .method("CLR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RM::CLR))
        .method("MOV", &SCAMP5RM::MOV)
        .method("MUX", &SCAMP5RM::MUX)
        .method("CLR_IF", &SCAMP5RM::CLR_IF)
        .method("REFRESH", &SCAMP5RM::REFRESH)
        .method("DNEWS0", &SCAMP5RM::DNEWS0)
        .method("DNEWS1", &SCAMP5RM::DNEWS1)
        .method("DNEWS", &SCAMP5RM::DNEWS)
        .method("scamp5_in", select_overload<void(AREG, int16_t)>(&SCAMP5RM::scamp5_in))
        .method("scamp5_in", select_overload<void(AREG, int16_t, AREG)>(&SCAMP5RM::scamp5_in))
        .method("scamp5_load_in", select_overload<void(AREG, int8_t, AREG)>(&SCAMP5RM::scamp5_load_in))
        .method("scamp5_load_in", select_overload<void(AREG, int8_t)>(&SCAMP5RM::scamp5_load_in))
        .method("scamp5_load_in", select_overload<void(int8_t)>(&SCAMP5RM::scamp5_load_in))
        .method("scamp5_load_dac", select_overload<void(AREG, uint16_t, AREG)>(&SCAMP5RM::scamp5_load_dac))
        .method("scamp5_load_dac", select_overload<void(AREG, uint16_t)>(&SCAMP5RM::scamp5_load_dac))
        .method("scamp5_load_dac", select_overload<void(uint16_t)>(&SCAMP5RM::scamp5_load_dac))
        .method("scamp5_read_areg", &SCAMP5RM::scamp5_read_areg)
        .method("scamp5_global_or", &SCAMP5RM::scamp5_global_or)(default_arguments((uint8_t)0, (uint8_t)0, (uint8_t)255, (uint8_t)255))
        .method("scamp5_global_count", &SCAMP5RM::scamp5_global_count)(default_arguments(0))
        .method("scamp5_load_point", &SCAMP5RM::scamp5_load_point)
        .method("scamp5_load_rect", &SCAMP5RM::scamp5_load_rect)
        .method("scamp5_load_pattern", &SCAMP5RM::scamp5_load_pattern)
        .method("scamp5_select_point", &SCAMP5RM::scamp5_select_point)
        .method("scamp5_select_rect", &SCAMP5RM::scamp5_select_rect)
        .method("scamp5_select_pattern", &SCAMP5RM::scamp5_select_pattern)
        .method("scamp5_select_col", &SCAMP5RM::scamp5_select_col)
        .method("scamp5_select_row", &SCAMP5RM::scamp5_select_row)
        .method("scamp5_select_colx", &SCAMP5RM::scamp5_select_colx)
        .method("scamp5_select_rowx", &SCAMP5RM::scamp5_select_rowx)
        .method("scamp5_draw_begin", &SCAMP5RM::scamp5_draw_begin)
        .method("scamp5_draw_end", &SCAMP5RM::scamp5_draw_end)
        .method("scamp5_draw_pixel", &SCAMP5RM::scamp5_draw_pixel)
        .method("scamp5_draw_point", &SCAMP5RM::scamp5_draw_point)
        .method("scamp5_draw_negate", &SCAMP5RM::scamp5_draw_negate)
        .method("scamp5_scan_areg", &SCAMP5RM::scamp5_scan_areg)
        .method("scamp5_scan_areg_8x8", &SCAMP5RM::scamp5_scan_areg_8x8)
        .method("scamp5_scan_dreg", &SCAMP5RM::scamp5_scan_dreg)(default_arguments((uint8_t)0, (uint8_t)255))
        .method("scamp5_scan_events", select_overload<void(DREG, uint8_t*, uint16_t, uint8_t, uint8_t)>(&SCAMP5RM::scamp5_scan_events))(default_arguments((uint16_t)1000, (uint8_t)0, (uint8_t)0))
        .method("scamp5_scan_events", select_overload<void(DREG, uint8_t*, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)>(&SCAMP5RM::scamp5_scan_events))
        .method("print_stats", &SCAMP5RM::print_stats);
}

