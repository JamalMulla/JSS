//
// Created by jm1417 on 01/04/2021.
//

#include "../../scamp5/scamp5.h"

#include <catch2/catch.hpp>

#include "../../tests/utility.h"
#include "simulator/util/utility.h"

TEST_CASE("DNEWS0 is working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s;
    DigitalRegister out(rows, cols);

    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

    // These control movement of values in each direction
    cv::Mat R_NORTH = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 1, 0,
                                1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0);
    cv::Mat R_EAST = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1);
    cv::Mat R_SOUTH = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
                                0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0);
    s.RN->write(R_NORTH);
    s.RE->write(R_EAST);
    s.RS->write(R_SOUTH);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
                                 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);

    s.DNEWS0(&out, &d);
    REQUIRE(utility::are_mats_equal(out.value(), expected));
}
