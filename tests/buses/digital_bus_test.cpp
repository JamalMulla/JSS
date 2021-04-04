//
// Created by jm1417 on 01/04/2021.
//

#include "../../include/simulator/buses/digital_bus.h"

#include <catch2/catch.hpp>

#include "../../include/simulator/registers/analogue_register.h"
#include "../../include/simulator/util/utility.h"
#include "../utility.h"

TEST_CASE("two images can be xor'd") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,1,0, 1,1,0,0, 1,1,0,0, 0,0,1,1);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,1,0,0, 0,1,1,0, 1,0,0,1, 1,0,1,0);

    bus.XOR(A, A, B);
    DigitalRegister expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1,0,1,0, 1,0,1,0, 0,1,0,1, 1,0,0,1);
    REQUIRE(utility::mats_are_equal(A.value(), expected.value()));

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
        REQUIRE(utility::mats_are_equal(east.value(), expected_east));
    }

    SECTION("get_west()") {
        DigitalRegister west = DigitalRegister(rows, cols);
        bus.get_west(west, d, 1, 0, origin);
        cv::Mat expected_west =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 1, 0, 0, 0, 0, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(west.value(), expected_west));
    }

    SECTION("get_north()") {
        DigitalRegister north = DigitalRegister(rows, cols);
        bus.get_north(north, d, 1, 0, origin);
        cv::Mat expected_north =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0,
                      0, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(north.value(), expected_north));
    }

    SECTION("get_south()") {
        DigitalRegister south = DigitalRegister(rows, cols);
        bus.get_south(south, d, 1, 0, origin);
        cv::Mat expected_south =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 0);
        REQUIRE(utility::mats_are_equal(south.value(), expected_south));
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

        REQUIRE(utility::mats_are_equal(east.value(), expected_east));
    }

    SECTION("get_west()") {
        DigitalRegister west = DigitalRegister(rows, cols);
        bus.get_west(west, d, 1, 0, origin);
        cv::Mat expected_west =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 0, 0, 0,
                      1, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(west.value(), expected_west));
    }

    SECTION("get_north()") {
        DigitalRegister north = DigitalRegister(rows, cols);
        bus.get_north(north, d, 1, 0, origin);
        cv::Mat expected_north =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 0, 1, 0, 0,
                      0, 0, 1, 0, 0, 0, 0, 0);
        REQUIRE(utility::mats_are_equal(north.value(), expected_north));
    }

    SECTION("get_south()") {
        DigitalRegister south = DigitalRegister(rows, cols);
        bus.get_south(south, d, 1, 0, origin);
        cv::Mat expected_south =
            (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1, 0, 0, 0,
                      0, 0, 0, 0, 0, 1, 0, 0);
        REQUIRE(utility::mats_are_equal(south.value(), expected_south));
    }
}

TEST_CASE("images can be converted to digital superpixel format") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;

    AnalogueRegister a = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 3, 4, 1, 0,
                                   3, 3, 15, 15, 1, 10, 8, 7, 13, 11, 6, 15);
    DigitalRegister out(rows, cols);

    std::vector<std::vector<std::vector<int>>> bitorder = {
        {{1, 4}, {2, 3}},
    };
    std::unordered_map<int, cv::Point> locations;

    bus.positions_from_bitorder(bitorder, 1, 2, 2, locations);
    bus.superpixel_create(out, a, locations);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1,
                                 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0);

    REQUIRE(utility::mats_are_equal(out.value(), expected));
}

TEST_CASE("superpixel images can be shifted") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;
    Origin origin = Origin::TOP_RIGHT;

    DigitalRegister d = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 0, 0,
                                  0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0);
    DigitalRegister out(rows, cols);

    SECTION("shift right") {
        bus.superpixel_shift_right(out, d, origin);
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0, 0, 0, 0, 1,
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
        REQUIRE(utility::mats_are_equal(out.value(), expected));
    }

    SECTION("shift left") {
        bus.superpixel_shift_left(out, d, origin);
        cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,1,0,0);
        REQUIRE(utility::mats_are_equal(out.value(), expected));
    }
}

TEST_CASE("superpixel images can be added") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;
    Origin origin = Origin::TOP_RIGHT;

    DigitalRegister A = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,0,0,0, 0,0,1,0, 0,0,0,0, 1,0,1,0);
    DigitalRegister B = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 0,0,1,0, 0,1,0,0, 1,0,0,0);

    DigitalRegister out(rows, cols);

    bus.superpixel_add(out, A, B, origin);
    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,1,0,0, 0,0,0,0, 0,1,1,0, 0,1,1,0);
    utility::print_matrix<uint8_t>(expected);
    std::cout << "---------------" << std::endl;
    utility::print_matrix<uint8_t>(out.value());
    REQUIRE(utility::mats_are_equal(out.value(), expected));

}
