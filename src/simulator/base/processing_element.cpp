//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/processing_element.h"
#include <rttr/registration>
#include <utility>

ProcessingElement::ProcessingElement(int rows, int cols, int row_stride, int col_stride, Source source, const std::string &path, Config &config) :
    rows_(rows),
    cols_(cols),
    photodiode(Pixel(rows, cols, row_stride, col_stride, source, path, config)),
    config_(config)

{
//    // TODO need to be able to pass in some way of creating the underlying memory
//
//    for(int i = 0; i < num_analogue; i++) {
//        analogue_registers_.emplace_back(rows, cols, config);
//    }
//    for(int i = 0; i < num_digital; i++) {
//        digital_registers_.emplace_back(rows, cols, config);
//    }
}
#ifdef TRACK_STATISTICS
void ProcessingElement::update_static(double time) {

    for (auto& [_, a] : analogue_registers_) {
        a->update_static(time);
    }

    for (auto& [_, d] : digital_registers_) {
        d->update_static(time);
    }

    photodiode.update_static(time);
}

cv::Mat ProcessingElement::get_transistor_count() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_32S);

    for (auto& [_, a] : analogue_registers_) {
        out += a->get_transistor_count();
    }

    for (auto& [_, d] : digital_registers_) {
        out += d->get_transistor_count();
    }

    out += photodiode.get_transistor_count();
    return out;
}

cv::Mat ProcessingElement::get_static_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for (auto& [_, a] : analogue_registers_) {
        out += a->get_static_energy();
    }
    for (auto& [_, d] : digital_registers_) {
        out += d->get_static_energy();
    }
    out += photodiode.get_static_energy();
    return out;
}

cv::Mat ProcessingElement::get_dynamic_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for (auto& [_, a] : analogue_registers_) {
        out += a->get_dynamic_energy();
    }
    for (auto& [_, d] : digital_registers_) {
        out += d->get_dynamic_energy();
    }
    out += photodiode.get_dynamic_energy();
    return out;
}

void ProcessingElement::print_stats(const CycleCounter &counter) {
    std::cout << "====================" << "\n";
    for (auto& [_, a] : analogue_registers_) {
        a->print_stats(counter);
    }
    for (auto& [_, d] : digital_registers_) {
        d->print_stats(counter);
    }
    photodiode.print_stats(counter);
    std::cout << "====================" << "\n";
}

int ProcessingElement::get_cycle_count() {
    return 0;
}

void ProcessingElement::add_analogue_register(const std::string &name, std::shared_ptr<AnalogueRegister> reg) {
    analogue_registers_[name] = std::move(reg);
}

void ProcessingElement::add_digital_register(const std::string &name, std::shared_ptr<DigitalRegister> reg) {
    digital_registers_[name] = std::move(reg);
}

std::shared_ptr<AnalogueRegister> ProcessingElement::get_analogue_register(const std::string &name) {
    return analogue_registers_[name];
}

std::shared_ptr<DigitalRegister> ProcessingElement::get_digital_register(const std::string &name) {
    return digital_registers_[name];
}

//void ProcessingElement::print_stats(const CycleCounter &counter) {
//    for(auto &analogue: analogue_registers_) { analogue.print_stats(counter); }
//    for(auto &digital: digital_registers_) { digital.print_stats(counter); }
//}
//
//void ProcessingElement::write_stats(const CycleCounter &counter, json &j) {
//    double total_analogue = 0;
//    double total_digital = 0;
//    double runtime = counter.to_seconds(stats::CLOCK_RATE);
//    auto analogues = json::array();
//    auto digitals = json::array();
//    for(auto &analogue: analogue_registers_) {
//        analogue.write_stats(counter, analogues);
//        total_analogue += analogue.get_total_energy();
//    }
//    for(auto &digital: digital_registers_) {
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

rttr::variant ProcessingElement::builder::analogue_registers_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<AnalogueRegister> > analogue_registers;

    for (auto& [_, value] : j.items()) {
        std::shared_ptr<AnalogueRegister> reg = std::make_shared<AnalogueRegister>(rows_, cols_, config_);
        reg->name_ = value;
        analogue_registers[value] = reg;
    }
    return rttr::variant(analogue_registers);
}

rttr::variant ProcessingElement::builder::digital_registers_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<DigitalRegister> > digital_registers;

    for (auto& [_, value] : j.items()) {
        std::shared_ptr<DigitalRegister> reg = std::make_shared<DigitalRegister>(rows_, cols_, config_);
        reg->name_ = value;
        digital_registers[value] = reg;
    }
    return rttr::variant(digital_registers);
}

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

ProcessingElement::builder &ProcessingElement::builder::with_analogue_registers(std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers) {
    this->analogue_registers_ = std::move(analogue_registers);
    return *this;
}

ProcessingElement::builder &ProcessingElement::builder::with_digital_registers(std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers) {
    this->digital_registers_ = std::move(digital_registers);
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
    ProcessingElement pe = ProcessingElement(rows_, cols_, row_stride_, col_stride_, source_, path_, config_);
    for (auto& [name, reg] : analogue_registers_) {
        pe.add_analogue_register(name, reg);
    }
    for (auto& [name, reg] : digital_registers_) {
        pe.add_digital_register(name, reg);
    }
    return pe;
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ProcessingElement::builder>("ProcessingElement_builder")
        .constructor<>()
        .method("analogue_registers_converter", &ProcessingElement::builder::analogue_registers_converter)
        .method("digital_registers_converter", &ProcessingElement::builder::digital_registers_converter)
        .method("with_rows", &ProcessingElement::builder::with_rows)
        .method("with_cols", &ProcessingElement::builder::with_cols)
        .method("with_row_stride", &ProcessingElement::builder::with_row_stride)
        .method("with_col_stride", &ProcessingElement::builder::with_col_stride)
        .method("with_analogue_registers", &ProcessingElement::builder::with_analogue_registers)
        .method("with_digital_registers", &ProcessingElement::builder::with_digital_registers)
        .method("with_input_source", &ProcessingElement::builder::with_input_source)
        .method("with_file_path", &ProcessingElement::builder::with_file_path)
        .method("build", &ProcessingElement::builder::build);

    registration::class_<ProcessingElement>("ProcessingElement");

};
