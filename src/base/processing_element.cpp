//
// Created by jm1417 on 28/01/2021.
//

#include "processing_element.h"

ProcessingElement::ProcessingElement(
        int rows,
        int columns,
        int num_analogue,
        int num_digital
        )
    : photodiode(Photodiode(rows, columns)){

    for (int i = 0; i < num_analogue; i++) {
        analogue_registers.emplace_back(rows, columns);
    }
    for (int i = 0; i < num_digital; i++) {
        digital_registers.emplace_back(rows, columns);
    }
}

void ProcessingElement::print_stats(const CycleCounter& counter) {
    for (auto & analogue : analogue_registers) {
        analogue.print_stats(counter);
    }
    for (auto & digital : digital_registers) {
        digital.print_stats(counter);
    }
}

void ProcessingElement::write_stats(const CycleCounter &counter, json& j) {
    for (auto & analogue : analogue_registers) {
        analogue.write_stats(counter, j);
    }
    for (auto & digital : digital_registers) {
        digital.write_stats(counter, j);
    }
}

ProcessingElement::builder &ProcessingElement::builder::with_rows(int rows) {
    this->rows_ = rows;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_cols(int cols) {
    this->cols_ = cols;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_analogue_registers(int num_analogue) {
    this->num_analogue_ = num_analogue;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_digital_registers(int num_digital) {
    this->num_digital_ = num_digital;
    return *this;
}

ProcessingElement ProcessingElement::builder::build() {
    return ProcessingElement(rows_, cols_, num_analogue_, num_digital_);
}
