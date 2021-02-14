//
// Created by jm1417 on 28/01/2021.
//

#include "processing_element.h"
#include "../metrics/stats.h"

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
    double total_analogue = 0;
    double total_digital = 0;
    double runtime = counter.to_seconds(stats::CLOCK_RATE);
    auto analogues = json::array();
    auto digitals = json::array();
    for (auto & analogue : analogue_registers) {
        analogue.write_stats(counter, analogues);
        total_analogue += analogue.get_total_energy();
    }
    for (auto & digital : digital_registers) {
        digital.write_stats(counter, digitals);
        total_digital += digital.get_total_energy();
    }

    j["Total analogue energy (J)"] = total_analogue;
    j["Total digital energy (J)"] = total_digital;
    j["Total analogue power (W)"] = total_analogue/runtime;
    j["Total digital power (W)"] = total_digital/runtime;
    j["Analogue registers"] = analogues;
    j["Digital registers"] = digitals;
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
