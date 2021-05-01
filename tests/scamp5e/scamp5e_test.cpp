//
// Created by jm1417 on 01/05/2021.
//

#include <catch2/catch.hpp>

#include "../../scamp5/scamp5.h"
#include "../../scamp5_extended/scamp5_e.h"
#include "../utility.h"

// Superpixel

TEST_CASE("shift patterns are generated correctly for arbitrary bitorders and different origins") {
    SECTION("4 bit boustrophedonic shift left pattern with BOTTOM_LEFT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;

        SCAMP5E s = SCAMP5E::builder {}
                        .with_rows(rows)
                        .with_cols(cols)
                        .with_origin(Origin::BOTTOM_LEFT)
                        .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 4},
             {2, 3}},
        };
        s.set_bitorder(bitorder);

        DigitalRegister RNORTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister REAST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST,
                                                  true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 1);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("4 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;

        SCAMP5E s = SCAMP5E::builder {}
                        .with_rows(rows)
                        .with_cols(cols)
                        .with_origin(Origin::TOP_RIGHT)
                        .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 4},
             {2, 3}},
        };

        s.set_bitorder(bitorder);

        DigitalRegister RNORTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister REAST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST, true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5E s = SCAMP5E::builder {}
                        .with_rows(rows)
                        .with_cols(cols)
                        .with_origin(Origin::TOP_RIGHT)
                        .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 8, 9, 16},
             {2, 7, 10, 15},
             {3, 6, 11, 14},
             {4, 5, 12, 13}},
        };

        s.set_bitorder(bitorder);

        DigitalRegister RNORTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister REAST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST, &RWEST,
                                                  true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift right pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5E s = SCAMP5E::builder {}
                        .with_rows(rows)
                        .with_cols(cols)
                        .with_origin(Origin::TOP_RIGHT)
                        .build();

        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 8, 9, 16},
             {2, 7, 10, 15},
             {3, 6, 11, 14},
             {4, 5, 12, 13}},
        };

        s.set_bitorder(bitorder);

        DigitalRegister RNORTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister REAST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST,
                                                  &RWEST, false);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0);

        REQUIRE(utility::mats_are_equal(RNORTH.read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST.read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH.read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST.read(), expected_w));
    }

    SECTION("16 bit spiral shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        SCAMP5E s = SCAMP5E::builder {}
                        .with_rows(rows)
                        .with_cols(cols)
                        .with_origin(Origin::TOP_RIGHT)
                        .build();

        // TODO double check
        Bitorder bitorder = {
            {{4, 3, 2, 1},
             {5, 14, 13, 12},
             {6, 15, 16, 11},
             {7, 8, 9, 10}},
        };

        s.set_bitorder(bitorder);

        DigitalRegister RNORTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister REAST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RSOUTH = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);
        DigitalRegister RWEST = (cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U);

        s.superpixel_shift_patterns_from_bitorder(bank, &RNORTH, &RSOUTH, &REAST,
                                                  &RWEST, true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);

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

    SCAMP5E s = SCAMP5E::builder {}
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

    s.set_bitorder(bitorder);

    s.superpixel_adc(&out, bank, &a);

    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1,
                              0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("1 bank superpixel images can be shifted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5E s = SCAMP5E::builder {}
                    .with_rows(rows)
                    .with_cols(cols)
                    .with_origin(Origin::TOP_RIGHT)
                    .build();

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    s.set_bitorder(bitorder);

    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
    DigitalRegister out(rows, cols);

    SECTION("shift right") {
        s.superpixel_shift_left(&out, bank, &d);
        auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }

    SECTION("shift left") {
        s.superpixel_shift_right(&out, bank, &d);
        auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(out.read(), expected));
    }
}

TEST_CASE("1 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5E s = SCAMP5E::builder {}
                    .with_rows(rows)
                    .with_cols(cols)
                    .with_origin(Origin::TOP_RIGHT)
                    .build();

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    s.set_bitorder(bitorder);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  1, 0, 1, 0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 1, 0, 0,
                                  1, 0, 0, 0);

    DigitalRegister out(rows, cols);

    s.superpixel_add(&out, bank, &A, &B);
    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                              0, 0, 0, 0,
                              0, 1, 1, 0,
                              0, 1, 1, 0);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("2 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5E s = SCAMP5E::builder {}
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

    s.set_bitorder(bitorder);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  1, 0, 1, 0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                                  0, 0, 0, 0,
                                  0, 1, 0, 0,
                                  1, 0, 0, 0);

    DigitalRegister out(rows, cols);

    s.superpixel_add(&out, bank, &A, &B);
    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                              0, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 1, 0, 0);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("1 bank superpixel images can be subtracted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    SCAMP5E s = SCAMP5E::builder {}
                    .with_rows(rows)
                    .with_cols(cols)
                    .with_origin(Origin::TOP_RIGHT)
                    .build();

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    s.set_bitorder(bitorder);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0);

    DigitalRegister out(rows, cols);

    s.superpixel_sub(&out, bank, &A, &B);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 0,
                                 0, 0, 0, 0);
    REQUIRE(utility::mats_are_equal(out.read(), expected));
}

TEST_CASE("superpixel images can be moved") {
    int rows = 4;
    int cols = 4;

    SCAMP5E s = SCAMP5E::builder {}
                    .with_rows(rows)
                    .with_cols(cols)
                    .with_origin(Origin::BOTTOM_LEFT)
                    .build();

    Bitorder bitorder = {
        {{1, 4},
         {2, 3}},
    };

    s.set_bitorder(bitorder);

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                  1, 0, 0, 0,
                                  1, 0, 1, 0,
                                  0, 0, 0, 0);

    DigitalRegister out(rows, cols);

    s.superpixel_movx(&out, &A, north);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                 0, 0, 0, 0,
                                 0, 0, 1, 0,
                                 1, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out.read(), expected));
}
