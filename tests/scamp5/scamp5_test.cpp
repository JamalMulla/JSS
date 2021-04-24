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


// Superpixel

TEST_CASE("shift patterns are generated correctly for arbitrary bitorders and different origins") {

    SECTION("4 bit boustrophedonic shift left pattern with BOTTOM_LEFT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;

        SCAMP5 s = SCAMP5::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_origin(Origin::BOTTOM_LEFT)
            .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {
                {1, 4},
                {2, 3}
            },
        };
        s.set_superpixel(bitorder, 2, 4);

        DigitalRegister RNORTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister REAST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST,
                                                    true);
        cv::Mat expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,1,0);
        cv::Mat expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0);
        cv::Mat expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0);
        cv::Mat expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,1);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("4 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;

        SCAMP5 s = SCAMP5::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_origin(Origin::TOP_RIGHT)
            .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {
                {1, 4},
                {2, 3}
            },
        };

        s.set_superpixel(bitorder, 2, 4);

        DigitalRegister RNORTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister REAST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST, true);
        cv::Mat expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0);
        cv::Mat expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,1);
        cv::Mat expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,1,0);
        cv::Mat expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5 s = SCAMP5::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_origin(Origin::TOP_RIGHT)
            .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {
                {1, 8, 9, 16},
                {2, 7, 10, 15},
                {3, 6, 11, 14},
                {4, 5, 12, 13}
            },
        };

        s.set_superpixel(bitorder, 4, 16);


        DigitalRegister RNORTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister REAST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST,
                                                    true);
        cv::Mat expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,1, 0,1,0,1, 0,1,0,1, 0,0,0,0);
        cv::Mat expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,1,0, 0,0,0,0, 0,0,0,0, 0,1,0,1);
        cv::Mat expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 1,0,1,0, 1,0,1,0, 1,0,1,0);
        cv::Mat expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift right pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5 s = SCAMP5::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_origin(Origin::TOP_RIGHT)
            .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {
                {1, 8, 9, 16},
                {2, 7, 10, 15},
                {3, 6, 11, 14},
                {4, 5, 12, 13}
            },
        };

        s.set_superpixel(bitorder, 4, 16);

        DigitalRegister RNORTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister REAST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST,
                                                    &RWEST, false);
        cv::Mat expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,1,0, 1,0,1,0, 1,0,1,0, 0,0,0,0);
        cv::Mat expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
        cv::Mat expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,1,0,1, 0,1,0,1, 0,1,0,1);
        cv::Mat expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 0,0,0,0, 0,0,0,0, 1,0,1,0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit spiral shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5 s = SCAMP5::builder {}
            .with_rows(rows)
            .with_cols(cols)
            .with_origin(Origin::TOP_RIGHT)
            .build();

        // TODO double check
        Bitorder bitorder = {
            {
                {4, 3, 2, 1},
                {5, 14, 13, 12},
                {6, 15, 16, 11},
                {7, 8, 9, 10}
            },
        };

        s.set_superpixel(bitorder, 4, 16);


        DigitalRegister RNORTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister REAST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat) cv::Mat::zeros(cv::Size(cols,rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST,
                                                  &RWEST, true);
        cv::Mat expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,1, 0,0,0,1, 0,0,0,0);
        cv::Mat expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,1,1,1);
        cv::Mat expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 1,0,0,0, 1,1,0,0, 1,0,0,0);
        cv::Mat expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,1,1,0, 0,1,1,0, 0,0,0,0, 0,0,0,0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

}

TEST_CASE("images can be converted to digital superpixel format") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    AnalogueRegister a = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 3, 4, 1, 0,
        3, 3, 15, 15, 1, 10, 8, 7, 13, 11, 6, 15);
    DigitalRegister out(rows, cols);

    Bitorder bitorder = {
        {{1, 4},
         {2, 3}},
    };

    s.set_superpixel(bitorder, 2, 4);

    s.superpixel_adc(&out, bank, &a);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1,
        0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("1 bank superpixel images can be shifted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    Bitorder bitorder = {
        {
            {1, 8, 9, 16},
            {2, 7, 10, 15},
            {3, 6, 11, 14},
            {4, 5, 12, 13}
        },
    };

    s.set_superpixel(bitorder, 4, 16);


    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
    DigitalRegister out(rows, cols);

    SECTION("shift right") {
        s.superpixel_shift_left(&out, bank, &d);
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("shift left") {
        s.superpixel_shift_right(&out, bank, &d);
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,1,0,0);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }
}

TEST_CASE("1 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    Bitorder bitorder = {
        {
            {1, 8, 9, 16},
            {2, 7, 10, 15},
            {3, 6, 11, 14},
            {4, 5, 12, 13}
        },
    };

    s.set_superpixel(bitorder, 4, 16);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,1,0, 0,0,0,0, 1,0,1,0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 0,0,1,0, 0,1,0,0, 1,0,0,0);

    DigitalRegister out(rows, cols);

    s.superpixel_add(&out, bank, &A, &B);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 0,0,0,0, 0,1,1,0, 0,1,1,0);
    std::cout << out.read() << std::endl;
    std::cout << expected << std::endl;
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("2 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    Bitorder bitorder = {
        {{1, 8, 0, 0},
         {2, 7, 0, 0},
         {3, 6, 0, 0},
         {4, 5, 0, 0}},
        {{0, 0, 1, 8},
         {0, 0, 2, 7},
         {0, 0, 3, 6},
         {0, 0, 4, 5}}};

    s.set_superpixel(bitorder, 4, 8);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) <<
        0,0,0,0,
        0,0,1,0,
        0,0,0,0,
        1,0,1,0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) <<
        0,1,0,0,
        0,0,0,0,
        0,1,0,0,
        1,0,0,0);

    DigitalRegister out(rows, cols);

    s.superpixel_add(&out, bank, &A, &B);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) <<
        1,1,0,0,
        0,0,0,0,
        0,1,0,0,
        1,0,0,0);
    std::cout << out.read() << std::endl;
    std::cout << expected << std::endl;
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("1 bank superpixel images can be subtracted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::TOP_RIGHT)
        .build();

    Bitorder bitorder = {
        {
            {1, 8, 9, 16},
            {2, 7, 10, 15},
            {3, 6, 11, 14},
            {4, 5, 12, 13}
        },
    };

    s.set_superpixel(bitorder, 4, 16);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,1,0, 0,0,0,0, 0,0,1,0, 0,0,0,0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,0,0);

    DigitalRegister out(rows, cols);

    s.superpixel_sub(&out, bank, &A, &B);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,1,0, 0,0,1,0, 0,0,0,0, 0,0,0,0);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("superpixel images can be moved") {
    int rows = 4;
    int cols = 4;

    SCAMP5 s = SCAMP5::builder {}
        .with_rows(rows)
        .with_cols(cols)
        .with_origin(Origin::BOTTOM_LEFT)
        .build();

    Bitorder bitorder = {
        {
            {1, 4, 0, 0},
            {2, 3, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        },
    };

    s.set_superpixel(bitorder, 2, 4);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) <<
        0,0,1,0,
        1,0,0,0,
        1,0,1,0,
        0,0,0,0);

    DigitalRegister out(rows, cols);

    s.superpixel_movx(&out, &A, north);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) <<
        0,0,0,0,
        0,0,0,0,
        0,0,1,0,
        1,0,0,0);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

