//
// Created by jm1417 on 05/05/2021.
//
#include <catch2/catch.hpp>
#include "simulator/memory/dram_array.h"

TEST_CASE("can write and read") {
    int rows = 4;
    int cols = 4;

    std::shared_ptr<Dram> dram = std::make_shared<Dram>();
    dram->set_rows(rows);
    dram->set_cols(cols);
    dram->set_row_stride(1);
    dram->set_col_stride(rows);
    dram->set_array_rows(2); // 4 rows in ram. 1 for each col
    dram->set_array_cols(16); // 16 bits in each row
    dram->init();

    SECTION("read/write single bits") {
        dram->write_bit(0, 0, 2, 1); // array 0, row 0, col 3
        REQUIRE(dram->read_bit(0, 0, 2) == 1);
        REQUIRE(dram->read_bit(0, 0, 1) == 0);
    }

    SECTION("read/write single bytes") {
        dram->write_byte(0, 0, 8, -69); // array 0, row 0, start_col 8
        dram->write_byte(0, 1, 0, 20); // array 0, row 1, start_col 0
        REQUIRE(dram->read_byte(0, 1, 0) == 20);
        REQUIRE(dram->read_byte(0, 0, 8) == -69);
        REQUIRE(dram->read_byte(0, 1, 1) == 10);
    }

}
