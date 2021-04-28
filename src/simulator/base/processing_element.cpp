//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/processing_element.h"

ProcessingElement::ProcessingElement(int rows, int cols, int row_stride, int col_stride, int num_analogue, int num_digital, Source source, const std::string &path, Config &config) :
    rows_(rows),
    cols_(cols),
    photodiode(Pixel(rows, cols, row_stride, col_stride, source, path, config)),
    config_(config)

{
    // TODO need to be able to pass in some way of creating the underlying memory
    for(int i = 0; i < num_analogue; i++) {
        analogue_registers.emplace_back(rows, cols, config);
    }
    for(int i = 0; i < num_digital; i++) {
        digital_registers.emplace_back(rows, cols, config);
    }
}
#ifdef TRACK_STATISTICS
void ProcessingElement::update_static(ulong cycles) {
    double time = (1 / (double) config_.clock_rate) * cycles;
    for(DigitalRegister &digital_register: digital_registers) {
        digital_register.update_static(time);
    }

    for(AnalogueRegister &analogue_register: analogue_registers) {
        analogue_register.update_static(time);
    }
    photodiode.update_static(time);
}

cv::Mat ProcessingElement::get_transistor_count() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_32S);
    for(auto &analogue: analogue_registers) {
        out += analogue.get_transistor_count();
    }
    for(auto &digital: digital_registers) {
        out += digital.get_transistor_count();
    }
    out += photodiode.get_transistor_count();
    return out;
}

cv::Mat ProcessingElement::get_static_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for(auto &analogue: analogue_registers) {
        out += analogue.get_static_energy();
    }
    for(auto &digital: digital_registers) {
        out += digital.get_static_energy();
    }
    out += photodiode.get_static_energy();
    return out;
}

cv::Mat ProcessingElement::get_dynamic_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for(auto &analogue: analogue_registers) {
        out += analogue.get_dynamic_energy();
    }
    for(auto &digital: digital_registers) {
        out += digital.get_dynamic_energy();
    }
    out += photodiode.get_dynamic_energy();
    return out;
}

void ProcessingElement::print_stats(const CycleCounter &counter) {
    std::cout << "====================" << "\n";
    for(auto &analogue: analogue_registers) { analogue.print_stats(counter); }
    for(auto &digital: digital_registers) { digital.print_stats(counter); }
    photodiode.print_stats(counter);
    std::cout << "====================" << "\n";
}

//void ProcessingElement::print_stats(const CycleCounter &counter) {
//    for(auto &analogue: analogue_registers) { analogue.print_stats(counter); }
//    for(auto &digital: digital_registers) { digital.print_stats(counter); }
//}
//
//void ProcessingElement::write_stats(const CycleCounter &counter, json &j) {
//    double total_analogue = 0;
//    double total_digital = 0;
//    double runtime = counter.to_seconds(stats::CLOCK_RATE);
//    auto analogues = json::array();
//    auto digitals = json::array();
//    for(auto &analogue: analogue_registers) {
//        analogue.write_stats(counter, analogues);
//        total_analogue += analogue.get_total_energy();
//    }
//    for(auto &digital: digital_registers) {
//        digital.write_stats(counter, digitals);
//        total_digital += digital.get_total_energy();
//    }
//
//    j["Total analogue energy (J)"] = total_analogue;
//    j["Total digital energy (J)"] = total_digital;
//    j["Total analogue power (W)"] = total_analogue / runtime;
//    j["Total digital power (W)"] = total_digital / runtime;
//    j["Analogue registers"] = analogues;
//    j["Digital registers"] = digitals;
//}
#endif

ProcessingElement::builder &ProcessingElement::builder::with_rows(int rows) {
    this->rows_ = rows;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_cols(int cols) {
    this->cols_ = cols;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_analogue_registers(
    int num_analogue) {
    this->num_analogue_ = num_analogue;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_digital_registers(
    int num_digital) {
    this->num_digital_ = num_digital;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_input_source(
    Source source) {
    this->source_ = source;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_file_path(
    const std::string &path) {
    this->path_ = path;
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_config(Config &config) {
    this->config_ = config;
    return *this;
}

ProcessingElement ProcessingElement::builder::build() {
#ifdef USE_RUNTIME_CHECKS
    if(rows_ < 0 || cols_ < 0 || row_stride_ < 0 || col_stride < 0 || num_analogue_ < 0 || num_digital_ < 0) {
        std::cerr << "ProcessingElement cannot be created as all necessary "
                     "parameters have not been set"
                  << std::endl;
    }
#endif
    return ProcessingElement(rows_, cols_, row_stride_, col_stride_, num_analogue_, num_digital_, source_, path_, config_);
}
