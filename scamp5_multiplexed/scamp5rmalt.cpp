//
// Created by jm1417 on 12/05/2021.
//

#include "scamp5rmalt.h"

#include <simulator/ui/ui.h>

void SCAMP5RMALT::init() {
    // Registers used often in instructions
    pe = this->get_component<ProcessingElement>("pe");
    alu = this->get_component<ALU>("alu");
    dram = this->get_component<Dram>("dram");
    adc = this->get_component<ADC>("adc");


    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            dram->write_byte(row_, col_, FLAG, 1);
        }
    }
}

void SCAMP5RMALT::nop() { this->update_cycles(1); }

void SCAMP5RMALT::rpix() {
    // reset *PIX
    this->pe->get_pixel()->reset();
    this->update_cycles(1);
}

void SCAMP5RMALT::motion() {
    get_image(C, D);

    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            this->dram->write_byte(row, col, E, 15);
            int c = this->dram->read_byte(row, col, C);
            int f = this->dram->read_byte(row, col, F);
            int c_f = this->alu->execute(c, f, ALU::SUB);
            int neg_d = this->alu->execute(0, c_f, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, neg_d);
            this->dram->write_byte(row, col, D, c_f);
            this->dram->write_byte(row, col, F, c_f);

            this->dram->write_byte(row, col, B, neg_d);

            this->alu->execute(c_f, 0, ALU::CMP);
            this->dram->write_bit(row, col, FLAG, !this->alu->N ? true : false);

            int news = this->dram->read_byte(row, col, NEWS);
            int neg = this->alu->execute(0, news, ALU::SUB);
            bool flag = this->dram->read_bit(row, col, FLAG);  //mask
            if (flag) {
                this->dram->write_byte(row, col, B, neg);
            }

            this->dram->write_bit(row, col, FLAG, 1);

            int e_val = this->dram->read_byte(row, col, E);
            int b_val = this->dram->read_byte(row, col, B);
            int sub = this->alu->execute(b_val, e_val, ALU::SUB);
            this->dram->write_byte(row, col, NEWS, -e_val);
            this->dram->write_byte(row, col, A, sub);

            int val = this->dram->read_byte(row, col, A);
            this->alu->execute(val, 0, ALU::CMP);
            this->dram->write_bit(row, col, FLAG, !this->alu->N ? true : false);

            this->dram->write_bit(row, col, R5, this->dram->read_bit(row, col, FLAG));

            this->dram->write_bit(row, col, FLAG, 1);

        }
    }
    alu->update_dynamic(6 * cols_);
    dram->update_dynamic((14 * 8 + 6) * cols_);
    this->update_cycles((12 + 14 * 8) * cols_);

}

void SCAMP5RMALT::get_image(AREG y, AREG h) {
    // y := full-range image, h := negative half-range image, and reset *PIX
    cv::Mat image = this->pe->get_pixel()->read();

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int16_t v = image.at<int16_t>(row_, col);
            dram->write_byte(row_, col_, PIX, v/2);
            dram->write_byte(row_, col_, NEWS, -v/2);
            dram->write_byte(row_, col_, h, -v/2);
            dram->write_byte(row_, col_, y, v);
        }
    }
    this->update_cycles(cols_ * 8 * 4 + cols_);
    adc->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->rpix();
    this->rpix();
    this->nop();
}


void SCAMP5RMALT::where(AREG a) {
    // FLAG := a > 0

    int val = this->dram->read_byte(row_, col_, a);
    this->alu->execute(val, 0, ALU::CMP);
    this->dram->write_bit(row_, col_, FLAG, !this->alu->N ? true : false);
    this->update_cycles(10);
    dram->update_dynamic(9);
    alu->update_dynamic(1);
}

// TODO use alu for comparison
void SCAMP5RMALT::where(AREG a0, AREG a1) {
    // FLAG := (a0 + a1) > 0.
    int a0_val = this->dram->read_byte(row_, col_, a0);
    int a1_val = this->dram->read_byte(row_, col_, a1);
    int res = this->alu->execute(a0_val, a1_val, ALU::ADD);
    this->alu->execute(res, 0, ALU::CMP);
    this->dram->write_bit(row_, col_, FLAG, !this->alu->N ? true : false);
    this->update_cycles(19);
    dram->update_dynamic( 17);
    alu->update_dynamic( 2);
}

// TODO use alu for comparison
void SCAMP5RMALT::where(AREG a0, AREG a1, AREG a2) {
    // FLAG := (a0 + a1 + a2) > 0.
    int a0_val = this->dram->read_byte(row_, col_, a0);
    int a1_val = this->dram->read_byte(row_, col_, a1);
    int a2_val = this->dram->read_byte(row_, col_, a2);
    int middle = this->alu->execute(a0_val, a1_val, ALU::ADD);
    int res = this->alu->execute(middle, a2_val, ALU::ADD);
    this->alu->execute(res, 0, ALU::CMP);
    this->dram->write_bit(row_, col_, FLAG, !this->alu->N ? true : false);
    this->update_cycles(28);
    alu->update_dynamic(3);
    dram->update_dynamic(25);
}

void SCAMP5RMALT::all() {
    // FLAG := 1.
    this->dram->write_bit(row_, col_, FLAG, 1);

    dram->update_dynamic(1);
    this->update_cycles( 1);
}

void SCAMP5RMALT::mov(AREG y, AREG x0) {
    // y = x0

    int x0_val = this->dram->read_byte(row_, col_, x0);
    int neg_x0 = this->alu->execute(0, x0_val, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg_x0);
    this->dram->write_byte(row_, col_, y, x0_val);

    dram->update_dynamic(8 * 3);
    alu->update_dynamic(1);
    this->update_cycles(8 * 3);
}

void SCAMP5RMALT::res(AREG a) {
    // a = 0
    this->dram->write_byte(row_, col_, NEWS, 0);
    this->dram->write_byte(row_, col_, a, 0);
    dram->update_dynamic(8 * 2);
    this->update_cycles(8 * 2);
}

// TODO cycles
void SCAMP5RMALT::res(AREG a, AREG b) {
    // a = 0, b := 0
    this->dram->write_byte(row_, col_, NEWS, 0);
    this->dram->write_byte(row_, col_, a, 0);
    this->dram->write_byte(row_, col_, b, 0);
    dram->update_dynamic(8 * 3);
    this->update_cycles( 8 * 3);
}

// TODO ALU
void SCAMP5RMALT::add(AREG y, AREG x0, AREG x1) {
    // y = x0 + x1

    int x0_val = this->dram->read_byte(row_, col_, x0);
    int x1_val = this->dram->read_byte(row_, col_, x1);
    int sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
    this->dram->write_byte(row_, col_, NEWS, -sum);
    this->dram->write_byte(row_, col_, y, sum);

    alu->update_dynamic(1);
    dram->update_dynamic(8 * 4);
    this->update_cycles( 8 * 4 + cols_);
}

// TODO ALU
void SCAMP5RMALT::add(AREG y, AREG x0, AREG x1, AREG x2) {
    // y = x0 + x1 + x2
    int x0_val = this->dram->read_byte(row_, col_, x0);
    int x1_val = this->dram->read_byte(row_, col_, x1);
    int x2_val = this->dram->read_byte(row_, col_, x2);
    int middle = this->alu->execute(x0_val, x1_val, ALU::ADD);
    int sum = this->alu->execute(middle, x2_val, ALU::ADD);
    this->dram->write_byte(row_, col_, NEWS, -sum);
    this->dram->write_byte(row_, col_, y, sum);

    alu->update_dynamic( 2);
    dram->update_dynamic( 8 * 5);
    this->update_cycles( 8 * 5 + 2);
}


void SCAMP5RMALT::sub(AREG y, AREG x0, AREG x1) {
    // y = x0 - x1

    int x0_val = this->dram->read_byte(row_, col_, x0);
    int x1_val = this->dram->read_byte(row_, col_, x1);
    int sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, -x0_val);
    this->dram->write_byte(row_, col_, y, sub);

    alu->update_dynamic(1);
    dram->update_dynamic( 8 * 4);
    this->update_cycles( 8 * 4 + cols_);

}

void SCAMP5RMALT::neg(AREG y, AREG x0) {
    // y = -x0
    int x0_val = this->dram->read_byte(row_, col_, x0);
    int neg = this->alu->execute(0, x0_val, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, 0);
    this->dram->write_byte(row_, col_, y, neg);

    alu->update_dynamic(1);
    dram->update_dynamic(8 * 3);
    this->update_cycles(8 * 3 + cols_ );
}

void SCAMP5RMALT::abs(AREG y, AREG x0) {
    // y = |x0|
    int x0_val = this->dram->read_byte(row_, col_, x0);
    int neg = this->alu->execute(0, x0_val, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg);
    this->dram->write_byte(row_, col_, y, neg);


    this->where(x0);


    int news = this->dram->read_byte(row_, col_, NEWS);
    neg = this->alu->execute(0, news, ALU::SUB);
    bool f = this->dram->read_bit(row_, col_, FLAG); //mask
    if (f) {
        this->dram->write_byte(row_, col_, y, neg);
    }

    this->all();
    alu->update_dynamic(2);
    dram->update_dynamic( 8 * 4);
    this->update_cycles( 8 * 4 + 2);
}


void SCAMP5RMALT::div(AREG y0, AREG y1, AREG y2) {
    // y0 := y2/2; y1 := -y2/2, y2 := y2

    int y1_val = this->dram->read_byte(row_, col_, y1);
    int y2_val = this->dram->read_byte(row_, col_, y2);
    int add = this->alu->execute(y1_val, y2_val, ALU::ADD);
    int neg = this->alu->execute(0, add, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg);

    int half_y2 = this->alu->execute(y2_val, 2, ALU::DIV);
    this->dram->write_byte(row_, col_, y0, half_y2);

    int neg_half_y2 = this->alu->execute(0, half_y2, ALU::SUB);
    this->dram->write_byte(row_, col_, y1, neg_half_y2);

    alu->update_dynamic(4);
    dram->update_dynamic( 8 * 5);
    this->update_cycles( 8 * 5 + 4);
}

void SCAMP5RMALT::div(AREG y0, AREG y1, AREG y2, AREG x0) {
    // y0 := 0.5*x0; y1 := -0.5*x0, y2 := x0

    int y1_val = this->dram->read_byte(row_, col_, y1);
    int x0_val = this->dram->read_byte(row_, col_, x0);
    this->dram->write_byte(row_, col_, y2, x0_val);

    int add = this->alu->execute(x0_val, y1_val, ALU::ADD);
    int neg = this->alu->execute(0, add, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg);

    int half_x0 = this->alu->execute(x0_val, 2, ALU::DIV);
    this->dram->write_byte(row_, col_, y0, half_x0);

    int neg_half_x0 = this->alu->execute(0, half_x0, ALU::SUB);
    this->dram->write_byte(row_, col_, y1, neg_half_x0);

    alu->update_dynamic(4);
    dram->update_dynamic(8 * 6);
    this->update_cycles(8 * 6 + 4);
}

void SCAMP5RMALT::diva(AREG y0, AREG y1, AREG y2) {
    // y0 := 0.5*y0; y1 := -0.5*y0, y2 := -0.5*y0

    int y0_val = this->dram->read_byte(row_, col_, y0);
    int y1_val = this->dram->read_byte(row_, col_, y1);

    int add = this->alu->execute(y0_val, y1_val, ALU::ADD);
    int neg = this->alu->execute(0, add, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg);

    int half_y0 = this->alu->execute(y0_val, 2, ALU::DIV);
    this->dram->write_byte(row_, col_, y0, half_y0);

    int neg_half_y0 = this->alu->execute(0, half_y0, ALU::SUB);
    this->dram->write_byte(row_, col_, y1, neg_half_y0);
    this->dram->write_byte(row_, col_, y2, neg_half_y0);

    alu->update_dynamic(4);
    dram->update_dynamic( 8 * 6);
    this->update_cycles( 8 * 6 + 4);
}

void SCAMP5RMALT::divq(AREG y0, AREG x0) {
    // y0 := 0.5*x0 + error

    int x0_val = this->dram->read_byte(row_, col_, x0);

    int half_x0 = this->alu->execute(x0_val, 2, ALU::DIV);
    int neg = this->alu->execute(0, half_x0, ALU::SUB);
    this->dram->write_byte(row_, col_, NEWS, neg);
    this->dram->write_byte(row_, col_, y0, half_x0);

    alu->update_dynamic(2);
    dram->update_dynamic( 8 * 3);
    this->update_cycles( 8 * 3 + 2);
}


void SCAMP5RMALT::movx(AREG y, AREG x0, news_t dir) {
    // y = x0_dir
    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int x0_val = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row_, col_, y, x0_val);
            this->dram->write_byte(row_, col_, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 3);
    this->update_cycles(cols_ * 8 * 3 + cols_ );
}

// If east or west, then same row_, same dram array. if north then north array, and same for south
void SCAMP5RMALT::mov2x(AREG y, AREG x0, news_t dir, news_t dir2) {
    // y = x0_dir_dir2 (note: this only works when FLAG is "all")

    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                    }
                    break;
                };
                case alldir: {
                    std::cerr << "Unhandled direction" << std::endl;
                    break;
                };
            }
            int neg = this->alu->execute(0, x0_val, ALU::SUB);
            this->dram->write_byte(row_, col_, NEWS, neg);

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
            this->dram->write_byte(row_, col_, y, x0_val);
        }
    }
    alu->update_dynamic(cols_);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ );
}

// If east or west, then same row_, same dram array. if north then north array, and same for south
void SCAMP5RMALT::addx(AREG y, AREG x0, AREG x1, news_t dir) {
    // y := x0_dir + x1_dir

    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int sum = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        int x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row_, dram_row_select, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        int x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row_, dram_row_select, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        int x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        int x1_val = this->dram->read_byte(dram_array_select, col_, x1);
                        sum = this->alu->execute(x0_val, x1_val, ALU::ADD);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        int x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        int x1_val = this->dram->read_byte(dram_array_select, col_, x1);
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
            this->dram->write_byte(row_, col_, y, sum);
            this->dram->write_byte(row_, col_, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ * 2);
}

// If east or west, then same row_, same dram array. if north then north array, and same for south
void SCAMP5RMALT::add2x(AREG y, AREG x0, AREG x1, news_t dir, news_t dir2) {
    // y := x0_dir_dir2 + x1_dir_dir2
    PlaneParams p;
    get_dir_params(p, dir, dir2, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int x1_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    dram_array_select = row;
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row_, dram_row_select, x1);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = row;
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row_, dram_row_select, x1);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    dram_row_select = col;
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        x1_val = this->dram->read_byte(dram_array_select, col_, x1);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = col;
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        x1_val = this->dram->read_byte(dram_array_select, col_, x1);
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
            this->dram->write_byte(row_, col_, NEWS, neg);

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
            this->dram->write_byte(row_, col_, y, this->alu->execute(x0_val, x1_val, ALU::ADD));
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 8 * 6);
    this->update_cycles(cols_ * 8 * 6 + cols_ * 3);
}

// If east or west, then same row_, same dram array. if north then north array, and same for south
void SCAMP5RMALT::subx(AREG y, AREG x0, news_t dir, AREG x1) {
    // y := x0_dir - x1
    PlaneParams p;
    get_dir_params(p, dir, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int sub = 0;
            switch (dir) {
                case east: {
                    int dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    if (dram_row_select >= 0) {
                        int x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row_, col_, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                }
                case west: {
                    int dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    if (dram_row_select < this->cols_) {
                        int x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        int x1_val = this->dram->read_byte(row_, col_, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                };
                case north: {
                    int dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    if (dram_array_select < this->rows_) {
                        int x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        int x1_val = this->dram->read_byte(row_, col_, x1);
                        sub = this->alu->execute(x0_val, x1_val, ALU::SUB);
                    }
                    break;
                };
                case south: {
                    int dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    if (dram_array_select >= 0) {
                        int x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        int x1_val = this->dram->read_byte(row_, col_, x1);
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
            this->dram->write_byte(row_, col_, y, sub);
            this->dram->write_byte(row_, col_, NEWS, neg);
        }
    }
    alu->update_dynamic(cols_ * 2);
    dram->update_dynamic(cols_ * 8 * 4);
    this->update_cycles(cols_ * 8 * 4 + cols_ * 2);
}

// If east or west, then same row_, same dram array. if north then north array, and same for south
void SCAMP5RMALT::sub2x(AREG y, AREG x0, news_t dir, news_t dir2, AREG x1) {
    // y := x0_dir_dir2 - x1
    PlaneParams p;
    get_dir_params(p, dir, dir2, 0, 0, this->rows_, this->cols_, 1, 1);

    for(int col = p.col_start; p.col_op(col_, p.col_end); col += p.col_step) {
        for(int row = p.row_start; p.row_op(row_, p.row_end); row += p.row_step) {
            int x0_val = 0;
            int x1_val = 0;
            int dram_row_select = 0;
            int dram_array_select = 0;
            switch (dir) {
                case east: {
                    dram_row_select = col - 1; // TOP_RIGHT origin means it's -1
                    dram_array_select = row;
                    if (dram_row_select < 0) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row_, col_, x1);
                    }
                    break;
                }
                case west: {
                    dram_row_select = col + 1; // TOP_RIGHT origin means it's +1
                    dram_array_select = row;
                    if (dram_row_select < this->cols_) {
                        x0_val = this->dram->read_byte(row_, dram_row_select, x0);
                        x1_val = this->dram->read_byte(row_, col_, x1);
                    }
                    break;
                };
                case north: {
                    dram_array_select = row - 1; // TOP_RIGHT origin means it's -1
                    dram_row_select = col;
                    if (dram_array_select < this->rows_) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        x1_val = this->dram->read_byte(row_, col_, x1);
                    }
                    break;
                };
                case south: {
                    dram_array_select = row + 1; // TOP_RIGHT origin means it's +1
                    dram_row_select = col;
                    if (dram_array_select >= 0) {
                        x0_val = this->dram->read_byte(dram_array_select, col_, x0);
                        x1_val = this->dram->read_byte(row_, col_, x1);
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
            this->dram->write_byte(row_, col_, NEWS, neg);

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
            this->dram->write_byte(row_, col_, y, this->alu->execute(x0_val, x1_val, ALU::SUB));
        }
    }
    alu->update_dynamic(cols_ * 3);
    dram->update_dynamic(cols_ * 8 * 5);
    this->update_cycles(cols_ * 8 * 5 + cols_ * 3);
}


void SCAMP5RMALT::OR(DREG d, DREG d0, DREG d1) {
    // d := d0 OR d1
    int res = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    this->dram->write_bit(row_, col_, d, res);
    alu->update_dynamic(1);
    dram->update_dynamic( 1 * 3);
    this->update_cycles( 1 * 3 + 1);
}


void SCAMP5RMALT::OR(DREG d, DREG d0, DREG d1, DREG d2) {
    // d := d0 OR d1 OR d2
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int res = this->alu->execute(d0_d1, dram->read_bit(row_, col_, d2), ALU::OR);
    this->dram->write_bit(row_, col_, d, res);
    alu->update_dynamic(2);
    dram->update_dynamic( 1 * 4);
    this->update_cycles( 1 * 4 + 2);
}

void SCAMP5RMALT::OR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {
    // d := d0 OR d1 OR d2 OR d3
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int d0_d1_d2 = this->alu->execute(d0_d1, dram->read_bit(row_, col_, d2), ALU::OR);
    int res = this->alu->execute(d0_d1_d2, dram->read_bit(row_, col_, d3), ALU::OR);
    this->dram->write_bit(row_, col_, d, res);
    alu->update_dynamic(3);
    dram->update_dynamic( 1 * 6);
    this->update_cycles( 1 * 6 + 3);
}


void SCAMP5RMALT::NOT(DREG d, DREG d0) {
    // d := NOT d0
    int negated = this->alu->execute(dram->read_bit(row_, col_, d0), 1, ALU::XOR);
    this->dram->write_bit(row_, col_, d, negated);
    alu->update_dynamic(1);
    dram->update_dynamic( 1 * 2);
    this->update_cycles(1 * 2 + 1);
}

void SCAMP5RMALT::NOR(DREG d, DREG d0, DREG d1) {
    // d := NOT(d0 OR d1)
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int negated = this->alu->execute(d0_d1, 1, ALU::XOR);
    this->dram->write_bit(row_, col_, d, negated);
    alu->update_dynamic( 2);
    dram->update_dynamic( 1 * 3);
    this->update_cycles(1 * 3 + 2);
}


void SCAMP5RMALT::NOR(DREG d, DREG d0, DREG d1, DREG d2) {
    // d := NOT(d0 OR d1 OR d2)
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int res = this->alu->execute(d0_d1, dram->read_bit(row_, col_, d2), ALU::OR);
    int negated = this->alu->execute(res, 1, ALU::XOR);
    this->dram->write_bit(row_, col_, d, negated);
    alu->update_dynamic(3);
    dram->update_dynamic( 1 * 4);
    this->update_cycles( 1 * 4 + 3);
}

void SCAMP5RMALT::NOR(DREG d, DREG d0, DREG d1, DREG d2, DREG d3) {
    // d := NOT(d0 OR d1 OR d2 OR d3)
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int d0_d1_d2 = this->alu->execute(d0_d1, dram->read_bit(row_, col_, d2), ALU::OR);
    int res = this->alu->execute(d0_d1_d2, dram->read_bit(row_, col_, d3), ALU::OR);
    int negated = this->alu->execute(res, 1, ALU::XOR);
    this->dram->write_bit(row_, col_, d, negated);
    alu->update_dynamic(4);
    dram->update_dynamic(1 * 5);
    this->update_cycles(1 * 5 + 4);
}

void SCAMP5RMALT::NOT(DREG Rl) {
    // Rl := NOT Rl
    this->NOT(Rl, Rl);
    this->update_cycles(4); // 2 reads, 1 op, 1 write
}

void SCAMP5RMALT::OR(DREG Rl, DREG Rx) {
    // Rl := Rl OR Rx
    this->OR(Rl, Rl, Rx);
}

void SCAMP5RMALT::NOR(DREG Rl, DREG Rx) {
    // Rl := Rl NOR Rx
    this->NOR(Rl, Rl, Rx);
}

void SCAMP5RMALT::AND(DREG Ra, DREG Rx, DREG Ry) {
    //  Ra := Rx AND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(R0);
    this->NOT(R12, Rx);
    this->NOT(R0, Ry);
    this->NOR(Ra, R0, R12);
}

void SCAMP5RMALT::NAND(DREG Ra, DREG Rx, DREG Ry) {
    // Ra := Rx NAND Ry; R0 = NOT Ry; R12 = NOT RX
    this->SET(R0);
    this->NOT(R12, Rx);
    this->NOT(R0, Ry);
    this->OR(Ra, R0, R12);
}

void SCAMP5RMALT::ANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rb AND Rx; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(R0, Rb);
    this->NOT(Rb, Rx);
    this->NOR(Ra, R0, Rb);
}

void SCAMP5RMALT::NANDX(DREG Ra, DREG Rb, DREG Rx) {
    // Ra := Rx NAND Ry; Rb := NOT Rx; R0 = NOT Rb
    this->NOT(R0, Rb);
    this->NOT(Rb, Rx);
    this->OR(Ra, R0, Rb);
}

void SCAMP5RMALT::IMP(DREG Rl, DREG Rx, DREG Ry) {
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

void SCAMP5RMALT::NIMP(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx NIMP Ry
    this->NOT(R0, Ry);
    this->NOR(R1, Rx, R0);
}

void SCAMP5RMALT::XOR(DREG Rl, DREG Rx, DREG Ry) {
    // Rl := Rx XOR Ry, Rx := *
    this->NOT(R0, Ry);
    this->NOR(Rl, Rx, R0);
    this->NOT(R0, Rx);
    this->NOR(Rx, Ry, R0);
    this->OR(Rl, Rx);
}

void SCAMP5RMALT::WHERE(DREG d) {
    // FLAG := d.
    this->dram->write_bit(row_, col_, FLAG, this->dram->read_bit(row_, col_, d));
    dram->update_dynamic(1 * 2);
    this->update_cycles(1 * 2);
}


void SCAMP5RMALT::WHERE(DREG d0, DREG d1) {
    // FLAG := d0 OR d1.
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    this->dram->write_bit(row_, col_, FLAG, d0_d1);
    alu->update_dynamic(1);
    dram->update_dynamic( 1 * 3);
    this->update_cycles( 1 * 3 + 1);
}


void SCAMP5RMALT::WHERE(DREG d0, DREG d1, DREG d2) {
    // FLAG := d0 OR d1 OR d2.
    int d0_d1 = this->alu->execute(dram->read_bit(row_, col_, d0), dram->read_bit(row_, col_, d1), ALU::OR);
    int res = this->alu->execute(d0_d1, dram->read_bit(row_, col_, d2), ALU::OR);
    this->dram->write_bit(row_, col_, FLAG, res);
    alu->update_dynamic(2);
    dram->update_dynamic(1 * 4);
    this->update_cycles( 1 * 4 + 2);
}


void SCAMP5RMALT::ALL() {
    // FLAG := 1, same as all.
    this->dram->write_bit(row_, col_, FLAG, 1);

    dram->update_dynamic(1);
    this->update_cycles( 1 * 3 + 1);
}


void SCAMP5RMALT::SET(DREG d0) {
    // d0 := 1
    this->dram->write_bit(row_, col_, d0, 1);

    dram->update_dynamic(1);
    this->update_cycles(1);
}

void SCAMP5RMALT::SET(DREG d0, DREG d1) {
    // d0, d1 := 1
    this->dram->write_bit(row_, col_, d0, 1);
    this->dram->write_bit(row_, col_, d1, 1);
    dram->update_dynamic(1 * 2);
    this->update_cycles( 1 * 2);
}

void SCAMP5RMALT::SET(DREG d0, DREG d1, DREG d2) {
    // 	d0, d1, d2 := 1
    this->dram->write_bit(row_, col_, d0, 1);
    this->dram->write_bit(row_, col_, d1, 1);
    this->dram->write_bit(row_, col_, d2, 1);
    dram->update_dynamic(1 * 3);
    this->update_cycles(1 * 3);
}

void SCAMP5RMALT::SET(DREG d0, DREG d1, DREG d2, DREG d3) {
    // d0, d1, d2, d3 := 1
    this->dram->write_bit(row_, col_, d0, 1);
    this->dram->write_bit(row_, col_, d1, 1);
    this->dram->write_bit(row_, col_, d2, 1);
    this->dram->write_bit(row_, col_, d3, 1);
    dram->update_dynamic(1 * 4);
    this->update_cycles(1 * 4);
}


void SCAMP5RMALT::CLR(DREG d0) {
    // d0 := 0
    this->dram->write_bit(row_, col_, d0, 0);

    dram->update_dynamic(1);
    this->update_cycles(1);
}

void SCAMP5RMALT::CLR(DREG d0, DREG d1) {
    // d0, d1 := 0
    this->dram->write_bit(row_, col_, d0, 0);
    this->dram->write_bit(row_, col_, d1, 0);

    dram->update_dynamic(1 * 2);
    this->update_cycles(1 * 2);
}

void SCAMP5RMALT::CLR(DREG d0, DREG d1, DREG d2) {
    // d0, d1, d2 := 0
    this->dram->write_bit(row_, col_, d0, 0);
    this->dram->write_bit(row_, col_, d1, 0);
    this->dram->write_bit(row_, col_, d2, 0);
    dram->update_dynamic(1 * 3);
    this->update_cycles(1 * 3);
}

void SCAMP5RMALT::CLR(DREG d0, DREG d1, DREG d2, DREG d3) {
    // 	d0, d1, d2, d3 := 0
    this->dram->write_bit(row_, col_, d0, 0);
    this->dram->write_bit(row_, col_, d1, 0);
    this->dram->write_bit(row_, col_, d2, 0);
    this->dram->write_bit(row_, col_, d3, 0);
    dram->update_dynamic(1 * 4);
    this->update_cycles(1 * 4);
}

void SCAMP5RMALT::MOV(DREG d, DREG d0) {
    // d := d0
    this->dram->write_bit(row_, col_, d, this->dram->read_bit(row_, col_, d0));
    dram->update_dynamic(1 * 2);
    this->update_cycles(1 * 2);
}

void SCAMP5RMALT::DNEWS0(DREG d, DREG d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 0 from the edge

    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_val = 0;

            if (this->dram->read_bit(row_, col_, R1)) {
                // south
                int dram_array_select = this->rows_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_array_select >= 0) {
                    d0_val = this->dram->read_bit(dram_array_select, col_, d0);
                }
            } else if (this->dram->read_bit(row_, col_, R2)) {
                // west
                int dram_row_select = this->cols_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_row_select < this->cols_) {
                    d0_val = this->dram->read_bit(row_, dram_row_select, d0);
                }
            }  else if (this->dram->read_bit(row_, col_, R3)) {
                // north
                int dram_array_select = this->rows_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_array_select < this->rows_) {
                    d0_val = this->dram->read_bit(dram_array_select, col_, d0);
                }
            } else if (this->dram->read_bit(row_, col_, R4)) {
                // east
                int dram_row_select = this->cols_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_row_select >= 0) {
                    d0_val = this->dram->read_bit(row_, dram_row_select, d0);
                }
            }

            this->dram->write_bit(row_, col_, d, d0_val);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}


void SCAMP5RMALT::DNEWS1(DREG d, DREG d0) {
    // d := d0_dir, direction selected by R1, R2, R3, R4
    // Reads 1 from the edge
    for (int row = 0; row < this->rows_; ++row) {
        for (int col = 0; col < this->cols_; ++col) {
            int d0_val = 1;

            if (this->dram->read_bit(row_, col_, R1)) {
                // south
                int dram_array_select = this->rows_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_array_select >= 0) {
                    d0_val = this->dram->read_bit(dram_array_select, col_, d0);
                }
            } else if (this->dram->read_bit(row_, col_, R2)) {
                // west
                int dram_row_select = this->cols_ + 1; // TOP_RIGHT origin means it's +1
                if (dram_row_select < this->cols_) {
                    d0_val = this->dram->read_bit(row_, dram_row_select, d0);
                }
            }  else if (this->dram->read_bit(row_, col_, R3)) {
                // north
                int dram_array_select = this->rows_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_array_select < this->rows_) {
                    d0_val = this->dram->read_bit(dram_array_select, col_, d0);
                }
            } else if (this->dram->read_bit(row_, col_, R4)) {
                // east
                int dram_row_select = this->cols_ - 1; // TOP_RIGHT origin means it's -1
                if (dram_row_select >= 0) {
                    d0_val = this->dram->read_bit(row_, dram_row_select, d0);
                }
            }

            this->dram->write_bit(row_, col_, d, d0_val);
        }
    }
    dram->update_dynamic(cols_ * 1 * 3);
    this->update_cycles(cols_ * 1 * 3);
}

void SCAMP5RMALT::DNEWS(DREG Ra, DREG Rx, int dir, bool boundary) {
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

void SCAMP5RMALT::scamp5_in(AREG areg, int16_t value, AREG temp) {
    // load an analog value to the AREG
    this->dram->write_byte(row_, col_, IN, value);
    this->dram->write_byte(row_, col_, temp, -value);
    this->dram->write_byte(row_, col_, areg, value);
    dram->update_dynamic(8 * 3);
    this->update_cycles(8 * 3);
}

void SCAMP5RMALT::scamp5_in(AREG areg, int16_t value) {
    // load an analog value to the AREG
    scamp5_in(areg, value, NEWS);
}

uint8_t SCAMP5RMALT::scamp5_read_areg(AREG areg, uint8_t r, uint8_t c) {
    // read a single pixel
    dram->update_dynamic(8);
    this->update_cycles(8);
    return this->dram->read_byte(r, c, areg);
}

void SCAMP5RMALT::display() {
    cv::Mat val_a = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_b = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_c = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_d = cv::Mat::zeros(cv::Size(rows_, cols_), CV_16S);
    cv::Mat val_r5 = cv::Mat::zeros(cv::Size(rows_, cols_), CV_8U);

    for (int row = 0; row < rows_; ++row) {
        for (int col = 0; col < cols_; ++col) {
            val_a.at<int16_t>(row_, col) = this->dram->read_byte(row_, col_, A);
            val_b.at<int16_t>(row_, col) = this->dram->read_byte(row_, col_, B);
            val_c.at<int16_t>(row_, col) = this->dram->read_byte(row_, col_, C);
            val_d.at<int16_t>(row_, col) = this->dram->read_byte(row_, col_, D);
            val_r5.at<uint8_t>(row_, col) = this->dram->read_bit(row_, col_, R5);
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

void SCAMP5RMALT::print_stats() {
    // TODO move
#ifdef TRACK_STATISTICS
    this->update_static(); //move
    Architecture::print_stats(rows_, cols_);

#endif
#ifndef TRACK_STATISTICS
    std::cerr << "Simulator has not been compiled with statistic tracking support. Recompile with -DTRACK_STATISTICS=ON" << std::endl;
#endif
}

rttr::variant SCAMP5RMALT::config_converter(json& j) {
    return Parser::get_instance().create_instance("Config", j);
}

void SCAMP5RMALT::set_rows(int rows) {
    this->rows_ = rows;
}

void SCAMP5RMALT::set_cols(int cols) {
    this->cols_ = cols;
}

void SCAMP5RMALT::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void SCAMP5RMALT::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void SCAMP5RMALT::set_origin(Origin origin) {
    this->origin_ = origin;
}

void SCAMP5RMALT::set_config(std::shared_ptr<Config> config) {
    this->config_ = std::move(config);
}

void SCAMP5RMALT::set_components(std::unordered_map<std::string, std::shared_ptr<Component> > components) {
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

    registration::class_<SCAMP5RMALT>("SCAMP5RMALT")
        .constructor()
        .method("motion", &SCAMP5RMALT::motion)
        .method("init", &SCAMP5RMALT::init)
        .method("display", &SCAMP5RMALT::display)
        .method("config_converter", &SCAMP5RMALT::config_converter)
        .method("set_rows", &SCAMP5RMALT::set_rows)
        .method("set_cols", &SCAMP5RMALT::set_cols)
        .method("set_row_stride", &SCAMP5RMALT::set_row_stride)
        .method("set_col_stride", &SCAMP5RMALT::set_col_stride)
        .method("set_origin", &SCAMP5RMALT::set_origin)
        .method("set_config", &SCAMP5RMALT::set_config)
        .method("set_components", &SCAMP5RMALT::set_components)
        .method("nop", &SCAMP5RMALT::nop)
        .method("rpix", &SCAMP5RMALT::rpix)
        .method("get_image", select_overload<void(AREG, AREG)>(&SCAMP5RMALT::get_image))
        .method("where", select_overload<void(AREG)>(&SCAMP5RMALT::where))
        .method("where", select_overload<void(AREG, AREG)>(&SCAMP5RMALT::where))
        .method("where", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RMALT::where))
        .method("all", &SCAMP5RMALT::all)
        .method("mov", &SCAMP5RMALT::mov)
        .method("res", select_overload<void(AREG)>(&SCAMP5RMALT::res))
        .method("res", select_overload<void(AREG, AREG)>(&SCAMP5RMALT::res))
        .method("add", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RMALT::add))
        .method("add", select_overload<void(AREG, AREG, AREG, AREG)>(&SCAMP5RMALT::add))
        .method("sub", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RMALT::sub))
        .method("neg", &SCAMP5RMALT::neg)
        .method("abs", &SCAMP5RMALT::abs)
        .method("div", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RMALT::div))
        .method("div", select_overload<void(AREG, AREG, AREG, AREG)>(&SCAMP5RMALT::div))
        .method("diva", select_overload<void(AREG, AREG, AREG)>(&SCAMP5RMALT::diva))
        .method("divq", select_overload<void(AREG, AREG)>(&SCAMP5RMALT::divq))
        .method("movx", &SCAMP5RMALT::movx)
        .method("mov2x", &SCAMP5RMALT::mov2x)
        .method("addx", &SCAMP5RMALT::addx)
        .method("add2x", &SCAMP5RMALT::add2x)
        .method("subx", &SCAMP5RMALT::subx)
        .method("sub2x", &SCAMP5RMALT::sub2x)
        .method("OR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RMALT::OR))
        .method("OR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::OR))
        .method("OR", select_overload<void(DREG, DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::OR))
        .method("NOT", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::NOT))
        .method("NOR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RMALT::NOR))
        .method("NOR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::NOR))
        .method("NOR", select_overload<void(DREG, DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::NOR))
        .method("NOR", select_overload<void(DREG)>(&SCAMP5RMALT::NOT))
        .method("OR", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::OR))
        .method("NOR", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::NOR))
        .method("AND", &SCAMP5RMALT::AND)
        .method("NAND", &SCAMP5RMALT::NAND)
        .method("ANDX", &SCAMP5RMALT::ANDX)
        .method("NANDX", &SCAMP5RMALT::NANDX)
        .method("IMP", &SCAMP5RMALT::IMP)
        .method("NIMP", &SCAMP5RMALT::NIMP)
        .method("XOR", &SCAMP5RMALT::XOR)
        .method("WHERE", select_overload<void(DREG)>(&SCAMP5RMALT::WHERE))
        .method("WHERE", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::WHERE))
        .method("WHERE", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RMALT::WHERE))
        .method("ALL", &SCAMP5RMALT::ALL)
        .method("SET", select_overload<void(DREG)>(&SCAMP5RMALT::SET))
        .method("SET", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::SET))
        .method("SET", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RMALT::SET))
        .method("SET", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::SET))
        .method("CLR", select_overload<void(DREG)>(&SCAMP5RMALT::CLR))
        .method("CLR", select_overload<void(DREG, DREG)>(&SCAMP5RMALT::CLR))
        .method("CLR", select_overload<void(DREG, DREG, DREG)>(&SCAMP5RMALT::CLR))
        .method("CLR", select_overload<void(DREG, DREG, DREG, DREG)>(&SCAMP5RMALT::CLR))
        .method("MOV", &SCAMP5RMALT::MOV)
        .method("DNEWS0", &SCAMP5RMALT::DNEWS0)
        .method("DNEWS1", &SCAMP5RMALT::DNEWS1)
        .method("DNEWS", &SCAMP5RMALT::DNEWS)
        .method("scamp5_in", select_overload<void(AREG, int16_t)>(&SCAMP5RMALT::scamp5_in))
        .method("scamp5_in", select_overload<void(AREG, int16_t, AREG)>(&SCAMP5RMALT::scamp5_in))
        .method("scamp5_read_areg", &SCAMP5RMALT::scamp5_read_areg)
        .method("print_stats", &SCAMP5RMALT::print_stats);
}
