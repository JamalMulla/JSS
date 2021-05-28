//
// Created by jm1417 on 03/04/2021.
//

#include "simulator/base/plane_params.h"
#include <rttr/registration>

RTTR_REGISTRATION {
    using namespace rttr;

    registration::enumeration<Origin>("Origin") (
        value("BOTTOM_LEFT", Origin::BOTTOM_LEFT),
        value("BOTTOM_RIGHT", Origin::BOTTOM_RIGHT),
        value("TOP_LEFT", Origin::TOP_LEFT),
        value("TOP_RIGHT", Origin::TOP_RIGHT)
    );

};

void get_fixed_params(PlaneParams& dst, Origin origin, int rb, int cb, int re,
                      int ce, int rs, int cs) {
    // rb = row start
    // cb = col start
    switch(origin) {
        case BOTTOM_LEFT : {
            dst.col_start = cb;
            dst.row_start = re;
            dst.col_end = ce;
            dst.row_end = rb;
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
            dst.col_start = cb;
            dst.row_start = rb;
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

void get_dir_params(PlaneParams& dst, news_t dir, int rb, int cb, int re, int ce, int rs, int cs) {
    switch(dir) {
        case north : {
            dst.col_start = cb;
            dst.row_start = re;
            dst.col_end = ce;
            dst.row_end = rb;
            dst.col_step = cs;
            dst.row_step = -rs;
            dst.col_op = std::less<>();
            dst.row_op = std::greater_equal<>();
            break;
        }
        case east: {
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
        case south: {
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
        case west: {
            dst.col_start = cb;
            dst.row_start = rb;
            dst.col_end = ce;
            dst.row_end = re;
            dst.col_step = cs;
            dst.row_step = rs;
            dst.col_op = std::less<>();
            dst.row_op = std::less<>();
            break;
        }
        case alldir: break;
    }
}


void get_dir_params(PlaneParams& dst, news_t dir, news_t dir2, int rb, int cb, int re, int ce, int rs, int cs) {
    dst.col_start = cb;
    dst.row_start = rb;
    dst.col_end = ce;
    dst.row_end = re;
    dst.col_step = cs;
    dst.row_step = rs;
    dst.col_op = std::less<>();
    dst.row_op = std::less<>();

    switch(dir) {
        case north : {
            dst.row_start = re;
            dst.row_end = rb;
            dst.row_step = -rs;
            dst.row_op = std::greater_equal<>();
            break;
        }
        case east: {
            dst.col_start = ce;
            dst.col_end = cb;
            dst.col_step = -cs;
            dst.col_op = std::greater_equal<>();
            break;
        }
        case south: {
            dst.row_start = rb;
            dst.row_end = re;
            dst.row_step = rs;
            dst.row_op = std::less<>();
            break;
        }
        case west: {
            dst.col_start = cb;
            dst.col_end = ce;
            dst.col_step = cs;
            dst.col_op = std::less<>();
            break;
        }
        case alldir: break;
    }

    switch(dir2) {
        case north : {
            dst.row_start = re;
            dst.row_end = rb;
            dst.row_step = -rs;
            dst.row_op = std::greater_equal<>();
            break;
        }
        case east: {
            dst.col_start = ce;
            dst.col_end = cb;
            dst.col_step = -cs;
            dst.col_op = std::greater_equal<>();
            break;
        }
        case south: {
            dst.row_start = rb;
            dst.row_end = re;
            dst.row_step = rs;
            dst.row_op = std::less<>();
            break;
        }
        case west: {
            dst.col_start = cb;
            dst.col_end = ce;
            dst.col_step = cs;
            dst.col_op = std::less<>();
            break;
        }
        case alldir: break;
    }

}
