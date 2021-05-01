//
// Created by jm1417 on 01/04/2021.
//

#include "../../scamp5/scamp5.h"

#include <catch2/catch.hpp>

#include "../../tests/utility.h"
#include "simulator/util/utility.h"

TEST_CASE("AND is working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();
    DigitalRegister out(rows, cols);

    DigitalRegister s1 = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,0,0, 0,0,0,0, 0,1,1,0, 1,0,1,0);
    DigitalRegister s2 = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 1,1,0,0, 0,1,1,0, 1,0,1,0);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,1,1,0, 1,0,1,0);

    s.AND(&out, &s1, &s2);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("XOR is working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();
    DigitalRegister out(rows, cols);

    DigitalRegister s1 = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,0,0, 0,0,0,0, 0,1,1,0, 1,0,1,0);
    DigitalRegister s2 = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 1,1,0,0, 0,1,1,0, 1,0,1,0);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0);

    s.XOR(&out, &s1, &s2);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("neighbour functions are working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    AnalogueRegister out(rows, cols);

    AnalogueRegister src = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
        29,10,20,-10,
        111,4,5,6,
        0,1,1,1,
        10,19,99,-90);


    SECTION("movx") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            111,4,5,6,
            0,1,1,1,
            10,19,99,-90,
            0,0,0,0);

        s.movx(&out, &src, north);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("mov2x") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            0,111,4,5,
            0,0,1,1,
            0,10,19,99,
            0,0,0,0);

        s.mov2x(&out, &src, north, east);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("addx") {
        AnalogueRegister src2 = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90);

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            0,0,0,0,
            47,38,2,8,
            122,15,27,25,
            9,0,-10,0);

        s.addx(&out, &src, &src2, south);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("add2x") {
        AnalogueRegister src2 = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90);

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            15,27,25,0,
            0,-10,0,0,
            0,108,0,0,
            0,0,0,0);

        s.add2x(&out, &src, &src2, west, north);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("subx") {
        AnalogueRegister src2 = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90);

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            -18, -28, 18, -18,
            18, -1, -2, -29,
            102, 5, 16, 7,
            -10, 20, -8, -89);


        s.subx(&out, &src, south, &src2);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("sub2x") {
        AnalogueRegister src2 = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90);

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            -14, -23, 24, -18,
            -10, -10, -21, -19,
            10, 100, -79, 1,
            -10, 19, -9, -90);

        s.sub2x(&out, &src, west, north, &src2);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

}



TEST_CASE("DNEWS0 is working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = SCAMP5::builder {}
                   .with_rows(rows)
                   .with_cols(cols)
                   .with_origin(Origin::TOP_RIGHT)
                   .build();
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
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}
