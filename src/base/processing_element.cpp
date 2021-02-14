//
// Created by jm1417 on 28/01/2021.
//

#include "processing_element.h"

#include <utility>

ProcessingElement::ProcessingElement(
        int rows,
        int columns,
        std::vector<AnalogueRegister>  analogue_regs,
        std::vector<DigitalRegister>  digital_regs
        )
    : photodiode(Photodiode(rows, columns)),
    analogue_registers(std::move(analogue_regs)),
    digital_registers(std::move(digital_regs)) {
}

void ProcessingElement::print_stats(CycleCounter counter) {
    for (auto & analogue : analogue_registers) {
        analogue.print_stats(counter);
    }
    for (auto & digital : digital_registers) {
        digital.print_stats(counter);
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

ProcessingElement::builder &ProcessingElement::builder::with_digital_registers(const std::vector<MemoryType> &types) {
    if (this->rows_ == -1 || this->cols_ == -1) {
        std::cerr << "Rows and/or columns have not been set" << std::endl;
        exit(1);
    }
    for (auto& type : types) {
        digital_regs_.emplace_back(this->rows_, this->cols_, type);
    }
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_analogue_registers(const std::vector<MemoryType> &types) {
    if (this->rows_ == -1 || this->cols_ == -1) {
        std::cerr << "Rows and/or columns have not been set" << std::endl;
        exit(1);
    }
    for (auto& type : types) {
        analogue_regs_.emplace_back(this->rows_, this->cols_, type);
    }
    return *this;
}

ProcessingElement ProcessingElement::builder::build() {
    return ProcessingElement(this->rows_, this->cols_, this->analogue_regs_, this->digital_regs_);
}
