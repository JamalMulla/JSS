//
// Created by jm1417 on 01/04/2021.
//

#include <catch2/catch.hpp>
#include <opencv2/core.hpp>
#include "../../include/simulator/registers/analogue_register.h"
#include "../../include/simulator/buses/digital_bus.h"
#include "../utility.h"
#include "../../include/simulator/util/utility.h"


TEST_CASE("images can be converted to digital superpixel format") {
    int rows = 4;
    int cols = 4;
    DigitalBus bus;

    AnalogueRegister a = (cv::Mat)(cv::Mat_<int16_t>(rows, cols) << 3,4,1,0,3,3,15,15,1,10,8,7,13,11,6,15);
    DigitalRegister out(rows, cols);

    std::vector<std::vector<std::vector<int> > > bitorder = {
            { {1, 4},
              {2, 3} },
    };
    std::unordered_map<int, cv::Point> locations;

    bus.positions_from_bitorder(bitorder, 1, 2, 2, locations);
    bus.convert_to_superpixel(a, out, locations);

    cv::Mat expected = (cv::Mat)(cv::Mat_<uint8_t>(rows, cols) << 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0);

    REQUIRE(utility::are_mats_equal(out.value(), expected));

}