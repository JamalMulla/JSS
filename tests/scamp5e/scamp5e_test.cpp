//
// Created by jm1417 on 01/05/2021.
//

#include <catch2/catch.hpp>
#include <utility>

#include "../../scamp5/scamp5.h"
#include "../../scamp5_extended/scamp5_e.h"
#include "../utility.h"

SCAMP5E setup(int rows, int cols, Bitorder bitorder, Origin origin) {

    std::shared_ptr<Config> config = std::make_shared<Config>(1e7);
    std::shared_ptr<Pixel> pixel = std::make_shared<Pixel>(rows, cols, 1, 1, LIVE, "", config);
    pixel->init();

    std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers;
    analogue_registers["PIX"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["IN"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["NEWS"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["A"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["B"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["C"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["D"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["E"] = std::make_shared<AnalogueRegister>(rows, cols, config);
    analogue_registers["F"] = std::make_shared<AnalogueRegister>(rows, cols, config);

    std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers;
    digital_registers["FLAG"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["SELECT"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["RECT"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R0"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R1"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R2"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R3"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R4"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R5"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R6"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R7"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R8"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R9"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R10"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R11"] = std::make_shared<DigitalRegister>(rows, cols, config);
    digital_registers["R12"] = std::make_shared<DigitalRegister>(rows, cols, config);


    std::shared_ptr<ProcessingElement> pe = std::make_shared<ProcessingElement>();
    pe->set_rows(rows);
    pe->set_cols(cols);
    pe->set_config(config);
    pe->set_pixel(pixel);
    pe->set_analogue_registers(analogue_registers);
    pe->set_digital_registers(digital_registers);

    std::shared_ptr<Dram> dram = std::make_shared<Dram>();

    SCAMP5E s = SCAMP5E();
    s.set_rows(rows);
    s.set_cols(cols);
    s.set_origin(origin);
    s.set_bitorder(std::move(bitorder));
    s.add_component("pe", pe);
    s.add_component("dram", dram);
    s.init();

    return s;
}



// Superpixel

TEST_CASE("shift patterns are generated correctly for arbitrary bitorders and different origins") {
    SECTION("4 bit boustrophedonic shift left pattern with BOTTOM_LEFT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;
        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 4},
             {2, 3}},
        };
        SCAMP5E s = setup(rows, cols, bitorder, BOTTOM_LEFT);

        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST,
                                                  true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 1);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));
    }

    SECTION("4 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 2;
        int cols = 2;
        int bank = 0;
        std::vector<std::vector<std::vector<int>>> bitorder = {
            {{1, 4},
             {2, 3}},
        };
        SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST, true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        Bitorder bitorder = {
            {{1, 8, 9, 16},
             {2, 7, 10, 15},
             {3, 6, 11, 14},
             {4, 5, 12, 13}},
        };

        SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST,true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));
    }

    SECTION("16 bit boustrophedonic shift right pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        Bitorder bitorder = {
            {{1, 8, 9, 16},
             {2, 7, 10, 15},
             {3, 6, 11, 14},
             {4, 5, 12, 13}},
        };

        SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);


        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST, false);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));
    }

    SECTION("16 bit spiral shift left pattern with TOP_RIGHT origin") {
        int rows = 4;
        int cols = 4;
        int bank = 0;

        Bitorder bitorder = {
                {{4, 3, 2, 1},
                {5, 14, 13, 12},
                {6, 15, 16, 11},
                {7, 8, 9, 10}},
        };

        SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST, true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));
    }

    SECTION("2-bank 8-bit bitorder") {
        int rows = 4;
        int cols = 4;
        int bank = 1;

        Bitorder bitorder = {
            {{1, 8, 0, 0},
            {2, 7, 0, 0},
            {3, 6, 0, 0},
            {4, 5, 0, 0}},
            {{0, 0, 1, 8},
            {0, 0, 2, 7},
            {0, 0, 3, 6},
            {0, 0, 4, 5}}};

        SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

        std::shared_ptr<DigitalRegister> RNORTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> REAST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RSOUTH = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));
        std::shared_ptr<DigitalRegister> RWEST = std::make_shared<DigitalRegister>((cv::Mat)cv::Mat::zeros(cv::Size(cols, rows), CV_8U));

        s.superpixel_shift_patterns_from_bitorder(bank, RNORTH, RSOUTH, REAST, RWEST, true);
        auto expected_n = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0);
        auto expected_s = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0);
        auto expected_e = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        auto expected_w = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

        REQUIRE(utility::mats_are_equal(RNORTH->read(), expected_n));
        REQUIRE(utility::mats_are_equal(REAST->read(), expected_e));
        REQUIRE(utility::mats_are_equal(RSOUTH->read(), expected_s));
        REQUIRE(utility::mats_are_equal(RWEST->read(), expected_w));


    }
}

TEST_CASE("images can be converted to digital superpixel format") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    Bitorder bitorder = {
        {{1, 4},
         {2, 3}},
    };

    SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

    std::shared_ptr<AnalogueRegister> a = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 3, 4, 1, 0, 3, 3, 15, 15, 1, 10, 8, 7, 13, 11, 6, 15));

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    s.superpixel_adc(out, bank, a);

    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1,
                              0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out->read(), expected));
}

TEST_CASE("1 bank superpixel images can be shifted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

    std::shared_ptr<DigitalRegister> d =  std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0));
    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);


    SECTION("shift right") {
        s.superpixel_shift_left(out, bank, d);
        auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
                                  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("shift left") {
        s.superpixel_shift_right(out, bank, d);
        auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }
}

TEST_CASE("1 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                  0, 0, 1, 0,
                  0, 0, 0, 0,
                  1, 0, 1, 0));

    std::shared_ptr<DigitalRegister> B = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                                  0, 0, 1, 0,
                                  0, 1, 0, 0,
                                  1, 0, 0, 0));

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    s.superpixel_add(out, bank, A, B);
    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                              0, 0, 0, 0,
                              0, 1, 1, 0,
                              0, 1, 1, 0);
    REQUIRE(utility::mats_are_equal(out->read(), expected));
}

TEST_CASE("2 bank superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    Bitorder bitorder = {
        {{1, 8, 0, 0},
         {2, 7, 0, 0},
         {3, 6, 0, 0},
         {4, 5, 0, 0}},
        {{0, 0, 1, 8},
         {0, 0, 2, 7},
         {0, 0, 3, 6},
         {0, 0, 4, 5}}};

    SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  1, 0, 1, 0));
    std::shared_ptr<DigitalRegister> B = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                                  0, 0, 0, 0,
                                  0, 1, 0, 0,
                                  1, 0, 0, 0));

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    s.superpixel_add(out, bank, A, B);
    auto expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0,
                              0, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 1, 0, 0);
    std::cout << out->read() << std::endl;
    REQUIRE(utility::mats_are_equal(out->read(), expected));
}

TEST_CASE("1 bank superpixel images can be subtracted") {
    int rows = 4;
    int cols = 4;
    int bank = 0;

    Bitorder bitorder = {
        {{1, 8, 9, 16},
         {2, 7, 10, 15},
         {3, 6, 11, 14},
         {4, 5, 12, 13}},
    };

    SCAMP5E s = setup(rows, cols, bitorder, TOP_RIGHT);

    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0));
    std::shared_ptr<DigitalRegister> B = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                  0, 0, 1, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0));

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    s.superpixel_sub(out, bank, A, B);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                 0, 0, 1, 0,
                                 0, 0, 0, 0,
                                 0, 0, 0, 0);
    REQUIRE(utility::mats_are_equal(out->read(), expected));
}

TEST_CASE("superpixel images can be moved") {
    int rows = 4;
    int cols = 4;

    Bitorder bitorder = {
        {{1, 4},
         {2, 3}},
    };

    SCAMP5E s = setup(rows, cols, bitorder, BOTTOM_LEFT);

    std::shared_ptr<DigitalRegister> A = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 1, 0,
                                  1, 0, 0, 0,
                                  1, 0, 1, 0,
                                  0, 0, 0, 0));

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    s.superpixel_movx(out, A, north);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0,
                                 0, 0, 0, 0,
                                 0, 0, 1, 0,
                                 1, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out->read(), expected));
}
