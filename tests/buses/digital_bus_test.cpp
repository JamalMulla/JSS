//
// Created by jm1417 on 01/04/2021.
//

#include "../../include/simulator/buses/digital_bus.h"

#include <catch2/catch.hpp>

#include "../../include/simulator/registers/analogue_register.h"
#include "../../include/simulator/util/utility.h"
#include "../utility.h"


TEST_CASE("bus operations") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,1,0, 1,1,0,0, 1,1,0,0, 0,0,1,1);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,1,0,0, 0,1,1,0, 1,0,0,1, 1,0,1,0);

    SECTION("NOT") {
        bus.NOT(A);
        DigitalRegister expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,0,1, 0,0,1,1, 0,0,1,1, 1,1,0,0);
        REQUIRE(utility::mats_are_equal(A.read(), expected.read()));
    }

    SECTION("XOR") {
        bus.XOR(A, A, B);
        DigitalRegister expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,1,0, 1,0,1,0, 0,1,0,1, 1,0,0,1);
        REQUIRE(utility::mats_are_equal(A.read(), expected.read()));
    }

    SECTION("NOR") {
        bus.NOR(A, A, B);
        DigitalRegister expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,1, 0,0,0,1, 0,0,1,0, 0,1,0,0);
        REQUIRE(utility::mats_are_equal(A.read(), expected.read()));
    }

}

TEST_CASE("digital image can be inverted") {
    DigitalBus bus;
    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(4, 4) << 0,1,1,0, 1,1,0,0, 1,1,0,0, 0,0,1,1);
    bus.NOT(A);
    DigitalRegister expected = (cv::Mat)(cv::Mat_<uint8_t>(4, 4) << 1,0,0,1, 0,0,1,1, 0,0,1,1, 1,1,0,0);
    REQUIRE(utility::mats_are_equal(A.read(), expected.read()));
}

TEST_CASE("get_X neighbour methods work correctly for BOTTOM_LEFT origin") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;
    Origin origin = Origin::BOTTOM_LEFT;

    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

    SECTION("get_east()") {
        DigitalRegister east = DigitalRegister(rows, cols);
        bus.get_east(east, d, 1, 0, origin);
        cv::Mat expected_east =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(east.read(), expected_east));
    }

    SECTION("get_west()") {
        DigitalRegister west = DigitalRegister(rows, cols);
        bus.get_west(west, d, 1, 0, origin);
        cv::Mat expected_west =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0, 0, 0, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(west.read(), expected_west));
    }

    SECTION("get_north()") {
        DigitalRegister north = DigitalRegister(rows, cols);
        bus.get_north(north, d, 1, 0, origin);
        cv::Mat expected_north =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0,
                      0, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(north.read(), expected_north));
    }

    SECTION("get_south()") {
        DigitalRegister south = DigitalRegister(rows, cols);
        bus.get_south(south, d, 1, 0, origin);
        cv::Mat expected_south =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 0);
        REQUIRE(utility::mats_are_equal(south.read(), expected_south));
    }
}

TEST_CASE("get_X neighbour methods work correctly for TOP_RIGHT origin") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;
    Origin origin = Origin::TOP_RIGHT;

    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);

    SECTION("get_east()") {
        DigitalRegister east = DigitalRegister(rows, cols);
        bus.get_east(east, d, 1, 0, origin);
        cv::Mat expected_east = (cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0, 0,
                                 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

        REQUIRE(utility::mats_are_equal(east.read(), expected_east));
    }

    SECTION("get_west()") {
        DigitalRegister west = DigitalRegister(rows, cols);
        bus.get_west(west, d, 1, 0, origin);
        cv::Mat expected_west =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(west.read(), expected_west));
    }

    SECTION("get_north()") {
        DigitalRegister north = DigitalRegister(rows, cols);
        bus.get_north(north, d, 1, 0, origin);
        cv::Mat expected_north =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 0);
        REQUIRE(utility::mats_are_equal(north.read(), expected_north));
    }

    SECTION("get_south()") {
        DigitalRegister south = DigitalRegister(rows, cols);
        bus.get_south(south, d, 1, 0, origin);
        cv::Mat expected_south =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0,
                      0, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(south.read(), expected_south));
    }
}
