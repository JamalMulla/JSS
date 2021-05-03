//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/processing_element.h"
#include "simulator/external/parser.h"
#include <rttr/registration>
#include <utility>

ProcessingElement::ProcessingElement(int rows, int cols, int row_stride, int col_stride, std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers, std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers, std::shared_ptr<Pixel> pixel, std::shared_ptr<Config> config)
: rows_(rows),
    cols_(cols),
    row_stride_(row_stride),
    col_stride_(col_stride),
    analogue_registers_(std::move(analogue_registers)),
    digital_registers_(std::move(digital_registers)),
    pixel_(std::move(pixel)),
    config_(std::move(config)) {}

rttr::variant ProcessingElement::analogue_registers_converter(json& j) {
    std::unordered_map<std::string, rttr::variant> analogue_registers;

    for (auto& [_, value] : j.items()) {
        std::shared_ptr<AnalogueRegister> reg = std::make_shared<AnalogueRegister>(rows_, cols_, config_);
        reg->name_ = value;
        analogue_registers[value] = reg;
    }
    return rttr::variant(analogue_registers);
}

rttr::variant ProcessingElement::digital_registers_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<DigitalRegister> > digital_registers;

    for (auto& [_, value] : j.items()) {
        std::shared_ptr<DigitalRegister> reg = std::make_shared<DigitalRegister>(rows_, cols_, config_);
        reg->name_ = value;
        digital_registers[value] = reg;
    }
    return rttr::variant(digital_registers);
}

#ifdef TRACK_STATISTICS
void ProcessingElement::update_static(double time) {

    for (auto& [_, a] : analogue_registers_) {
        a->update_static(time);
    }

    for (auto& [_, d] : digital_registers_) {
        d->update_static(time);
    }

    pixel_->update_static(time);
}

cv::Mat ProcessingElement::get_transistor_count() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_32S);

    for (auto& [_, a] : analogue_registers_) {
        out += a->get_transistor_count();
    }

    for (auto& [_, d] : digital_registers_) {
        out += d->get_transistor_count();
    }

    out += pixel_->get_transistor_count();
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
    out += pixel_->get_static_energy();
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
    out += pixel_->get_dynamic_energy();
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
    pixel_->print_stats(counter);
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

std::shared_ptr<Pixel> ProcessingElement::get_pixel() {
    return pixel_;
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


RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<ProcessingElement>("ProcessingElement")
        .constructor<int, int, int, int, std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>>, std::unordered_map<std::string, std::shared_ptr<DigitalRegister>>, std::shared_ptr<Pixel>, std::shared_ptr<Config>>()
        .method("analogue_registers_converter", &ProcessingElement::analogue_registers_converter)
        .method("digital_registers_converter", &ProcessingElement::digital_registers_converter);

};
