//
// Created by jm1417 on 01/04/2021.
//

#include "../../scamp5/scamp5.h"

#include <catch2/catch.hpp>

#include "../../tests/utility.h"
#include "simulator/util/utility.h"

SCAMP5 setup() {
    int rows = 4;
    int cols = 4;

    std::shared_ptr<Config> config = std::make_shared<Config>(1e7);
    std::shared_ptr<Pixel> pixel = std::make_shared<Pixel>(rows, cols, 1, 1, LIVE, "", config);

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

    SCAMP5 s = SCAMP5();
    s.set_rows(rows);
    s.set_cols(cols);
    s.set_origin(Origin::TOP_RIGHT);
    s.add_component("pe", pe);
    s.init();

    return s;
}

TEST_CASE("SCAMP5 Instructions") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = setup();

    std::shared_ptr<DigitalRegister> s1 = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,0,0, 0,0,0,0, 0,1,1,0, 1,0,1,0));
    std::shared_ptr<DigitalRegister> s2 = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 1,1,0,0, 0,1,1,0, 1,0,1,0));
    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    SECTION("AND") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,1,1,0, 1,0,1,0);
        s.AND(out, s1, s2);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("XOR") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,1,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0);
        s.XOR(out, s1, s2);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }


}


TEST_CASE("neighbour functions are working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = setup();

    std::shared_ptr<AnalogueRegister> out = std::make_shared<AnalogueRegister>(rows, cols);

    std::shared_ptr<AnalogueRegister> src = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
        29,10,20,-10,
        111,4,5,6,
        0,1,1,1,
        10,19,99,-90));


    SECTION("movx") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            111,4,5,6,
            0,1,1,1,
            10,19,99,-90,
            0,0,0,0);

        s.movx(out, src, north);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("mov2x") {
        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            0,111,4,5,
            0,0,1,1,
            0,10,19,99,
            0,0,0,0);

        s.mov2x(out, src, north, east);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("addx") {
        std::shared_ptr<AnalogueRegister> src2 = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90));

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            0,0,0,0,
            47,38,2,8,
            122,15,27,25,
            9,0,-10,0);

        s.addx(out, src, src2, south);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("add2x") {
        std::shared_ptr<AnalogueRegister> src2 = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90));

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            15,27,25,0,
            0,-10,0,0,
            0,108,0,0,
            0,0,0,0);

        s.add2x(out, src, src2, west, north);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("subx") {
        std::shared_ptr<AnalogueRegister> src2 = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90));

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            -18, -28, 18, -18,
            18, -1, -2, -29,
            102, 5, 16, 7,
            -10, 20, -8, -89);


        s.subx(out, src, south, src2);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

    SECTION("sub2x") {
        std::shared_ptr<AnalogueRegister> src2 = std::make_shared<AnalogueRegister>((cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            18,28,-18,18,
            11,11,22,19,
            9,-1,-11,-1,
            10,-19,9,90));

        cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) <<
            -14, -23, 24, -18,
            -10, -10, -21, -19,
            10, 100, -79, 1,
            -10, 19, -9, -90);

        s.sub2x(out, src, west, north, src2);
        REQUIRE(utility::mats_are_equal(out->read(), expected));
    }

}



TEST_CASE("DNEWS0 is working correctly") {
    int rows = 4;
    int cols = 4;
    SCAMP5 s = setup();

    std::shared_ptr<DigitalRegister> out = std::make_shared<DigitalRegister>(rows, cols);

    std::shared_ptr<DigitalRegister> d = std::make_shared<DigitalRegister>((cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0));

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

    s.DNEWS0(out, d);
    REQUIRE(utility::mats_are_equal(out->read(), expected));
}
