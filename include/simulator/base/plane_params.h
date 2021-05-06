//
// Created by jm1417 on 03/04/2021.
//

#ifndef SIMULATOR_PLANE_PARAMS_H
#define SIMULATOR_PLANE_PARAMS_H

#include <functional>

#include "../../../scamp5/news_t.h"

#define MAT_TYPE CV_16S

enum Origin { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_RIGHT, TOP_LEFT };

struct PlaneParams {
    int col_start;
    int row_start;
    int col_end;
    int row_end;
    int col_step;
    int row_step;
    std::function<bool(int, int)> col_op;
    std::function<bool(int, int)> row_op;
};

void get_fixed_params(PlaneParams& dst, Origin origin, int rb, int cb, int re,
                      int ce, int rs, int cs);

void get_dir_params(PlaneParams& dst, news_t dir, int rb, int cb, int re,
                    int ce, int rs, int cs);

void get_dir_params(PlaneParams& dst, news_t dir, news_t dir2, int rb, int cb, int re,
                    int ce, int rs, int cs);

#endif  // SIMULATOR_PLANE_PARAMS_H
