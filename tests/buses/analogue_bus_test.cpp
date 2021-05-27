//
// Created by jm1417 on 31/03/2021.
//

#include "../../include/simulator/buses/analogue_bus.h"

#include <catch2/catch.hpp>
#include <opencv2/core.hpp>

#include "../../include/simulator/registers/analogue_register.h"
#include "../../include/simulator/util/utility.h"
#include "../utility.h"

SCENARIO(
    "analogue bus correctly manipulates analogue registers when mask=all") {
    int rows = 3;
    int cols = 3;
    AnalogueBus bus;

    GIVEN("some number of analogue registers and a mask") {
        DigitalRegister mask = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 1,
                                         1, 1, 1, 1, 1, 1, 1);
        AnalogueRegister a = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 1, 2, 3,
                                       4, 5, 6, 7, 8, 9);
        AnalogueRegister dst(rows, cols);

        WHEN("just one register") {
            THEN("it is set to 0") {
                bus.bus(a, mask);
                REQUIRE(cv::sum(a.read())[0] == 0);
            }
        }

        WHEN("two registers dst and a") {
            THEN("dst = -a") {
                bus.bus(dst, a, mask);
                cv::Mat expected =
                    (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << -1, -2, -3, -4,
                              -5, -6, -7, -8, -9);
                REQUIRE(utility::mats_are_equal(dst.read(), expected));
            }
        }

        WHEN("3 registers, dst, a and b") {
            AnalogueRegister b = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 1,
                                           2, 3, 4, 5, 6, 7, 8, 9);
            THEN("dst = -(a + b)") {
                bus.bus(dst, a, b, mask);
                cv::Mat expected =
                    (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << -2, -4, -6, -8,
                              -10, -12, -14, -16, -18);
                REQUIRE(utility::mats_are_equal(dst.read(), expected));
            }
        }
    }
}

SCENARIO("digital registers can be correctly set based on simple conditions "
         "with analogue registers") {
    int rows = 3;
    int cols = 3;
    AnalogueBus bus;

    GIVEN("some number of analogue registers and a digital register") {
        AnalogueRegister a = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << -1, 2,
                                       3, -1, 5, 6, -1, 8, 9);

        WHEN("1 analogue register") {
            DigitalRegister b(rows, cols);
            THEN("b = 1 if a > 0") {
                bus.conditional_positive_set(b, a);
                cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 0,
                                             1, 1, 0, 1, 1, 0, 1, 1);
                REQUIRE(utility::mats_are_equal(b.read(), expected));
            }
        }
    }
}

SCENARIO("analogue registers can be moved via the bus") {
    int rows = 3;
    int cols = 3;
    AnalogueBus bus;

    GIVEN("some number of analogue registers") {
        DigitalRegister mask = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 1,
                                         1, 1, 1, 1, 1, 1, 1);
        AnalogueRegister src = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 1, 2,
                                         3, 4, 5, 6, 7, 8, 9);

        WHEN("1 src, 1 dst") {
            AnalogueRegister intermediate(rows, cols);
            AnalogueRegister dst(rows, cols);
            THEN("dst = src") {
                bus.mov(dst, src, intermediate, mask);
                cv::Mat expected = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 1,
                                             2, 3, 4, 5, 6, 7, 8, 9);
                REQUIRE(utility::mats_are_equal(dst.read(), expected));
            }
        }
    }
}
