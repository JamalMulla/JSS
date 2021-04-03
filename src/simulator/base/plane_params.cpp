//
// Created by jm1417 on 03/04/2021.
//

#include <simulator/base/plane_params.h>

void get_fixed_params(PlaneParams& dst, Origin origin, int rb, int cb, int re,
                      int ce, int rs, int cs) {
    switch(origin) {
        case BOTTOM_LEFT : {
            dst.col_start = cb;
            dst.row_start = re;
            dst.col_end = ce;
            dst.row_end = rs;
            dst.col_step = cs;
            dst.row_step = -rs;
            dst.col_op = std::less<>();
            dst.row_op = std::greater_equal<>();
            break;
        }
        case BOTTOM_RIGHT: {
            dst.col_start = ce;
            dst.row_start = re;
            dst.col_end = cb;
            dst.row_end = re;
            dst.col_step = -cs;
            dst.row_step = -rs;;
            dst.col_op = std::greater_equal<>();
            dst.row_op = std::greater_equal<>();
            break;
        }
        case TOP_RIGHT: {
            dst.col_start = ce;
            dst.row_start = rb;
            dst.col_end = cb;
            dst.row_end = re;
            dst.col_step = -cs;
            dst.row_step = rs;
            dst.col_op = std::greater_equal<>();
            dst.row_op = std::less<>();
            break;
        }
        case TOP_LEFT: {
            // this is what OpenCV uses
            dst.col_start = cs;
            dst.row_start = rs;
            dst.col_end = ce;
            dst.row_end = re;
            dst.col_step = cs;
            dst.row_step = rs;
            dst.col_op = std::less<>();
            dst.row_op = std::less<>();
            break;
        }
    }
}
