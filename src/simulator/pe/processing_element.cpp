//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/pe/processing_element.h"

#include <rttr/registration>
#include <utility>

#include "simulator/external/parser.h"

void ProcessingElement::set_rows(int rows) {
    this->rows_ = rows;
}

void ProcessingElement::set_cols(int cols) {
    this->cols_ = cols;
}

void ProcessingElement::set_row_stride(int row_stride) {
    this->row_stride_ = row_stride;
}

void ProcessingElement::set_col_stride(int col_stride) {
    this->col_stride_ = col_stride;
}

void ProcessingElement::set_analogue_registers(std::unordered_map<std::string, std::shared_ptr<AnalogueRegister>> analogue_registers) {
    this->analogue_registers_ = std::move(analogue_registers);
}

void ProcessingElement::set_digital_registers(std::unordered_map<std::string, std::shared_ptr<DigitalRegister>> digital_registers) {
    this->digital_registers_ = std::move(digital_registers);
}

void ProcessingElement::set_pixel(std::shared_ptr<Pixel> pixel) {
    this->pixel_ = std::move(pixel);
}

void ProcessingElement::set_config(std::shared_ptr<Config> config) {
    this->config_ = std::move(config);
}

rttr::variant ProcessingElement::analogue_registers_converter(json& j, std::unordered_map<std::string, rttr::variant>& cache) {
    for (auto& [_, value]: j.items()) {
        std::shared_ptr<AnalogueRegister> reg = std::make_shared<AnalogueRegister>(rows_, cols_, config_);
        reg->name_ = value;
        analogue_registers_[value] = reg;
        cache[value] = reg;
    }
    return rttr::variant(analogue_registers_);
}

rttr::variant ProcessingElement::digital_registers_converter(json& j, std::unordered_map<std::string, rttr::variant>& cache) {

    for (auto& [_, value]: j.items()) {
        std::shared_ptr<DigitalRegister> reg = std::make_shared<DigitalRegister>(rows_, cols_, config_);
        // register has a mask
        if (value.is_object()) {
            if (!value.contains("name")) {
                std::cerr << "No name field in register object " << value.dump(2) << std::endl;
                exit(EXIT_FAILURE);
            }
            if (!value.contains("mask")) {
                std::cerr << "No mask field in register object " << value.dump(2) << std::endl;
                exit(EXIT_FAILURE);
            }
            reg->name_ = value["name"];
            std::shared_ptr<DigitalRegister> mask = get_digital_register(value["mask"]);
            if (!mask) {
                std::cerr << "Could not get mask register " << value["mask"] << std::endl;
                exit(EXIT_FAILURE);
            }
            reg->set_mask(mask);
        } else {
            reg->name_ = value;
        }
        digital_registers_[reg->name_] = reg;
        cache[reg->name_] = reg;
    }
    return rttr::variant(digital_registers_);
}

rttr::variant ProcessingElement::pixel_converter(json& j) {
    Source source = Source::LIVE;
    std::string path;

    if (j.contains("input_source")) {
        std::string src = j["input_source"];
        if (src == "LIVE") { }
        else if (src == "VIDEO") {
            source = Source::VIDEO;
            if (!j.contains("path")) {
                std::cerr << "Path not specified for VIDEO read. Add a \"path\" field." << std::endl;
                exit(EXIT_FAILURE);
            }
            path = j["path"];
        } else if (src == "IMAGE") {
            source = Source::IMAGE;
            if (!j.contains("path")) {
                std::cerr << "Path not specified for IMAGE read. Add a \"path\" field." << std::endl;
                exit(EXIT_FAILURE);
            }
            path = j["path"];
        } else {
            std::cout << "Unknown input. Assuming LIVE input" << std::endl;
        }
    }

    return rttr::variant(std::make_shared<Pixel>(rows_, cols_, row_stride_, col_stride_, source, path, config_));
}


void ProcessingElement::add_analogue_register(const std::string& name, std::shared_ptr<AnalogueRegister> reg) {
    analogue_registers_[name] = std::move(reg);
}

void ProcessingElement::add_digital_register(const std::string& name, std::shared_ptr<DigitalRegister> reg) {
    digital_registers_[name] = std::move(reg);
}

std::shared_ptr<AnalogueRegister> ProcessingElement::get_analogue_register(const std::string& name) {
    if (digital_registers_.find(name) == digital_registers_.end()) {
        // not found
        std::cerr << "[Error] Could not find analogue register \"" << name << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    return analogue_registers_[name];
}

std::shared_ptr<DigitalRegister> ProcessingElement::get_digital_register(const std::string& name) {
    if (digital_registers_.find(name) == digital_registers_.end()) {
        // not found
        std::cerr << "[Error] Could not find digital register \"" << name << "\". Make sure it has been added" << std::endl;
        exit(EXIT_FAILURE);
    }
    return digital_registers_[name];
}

std::shared_ptr<Pixel> ProcessingElement::get_pixel() {
    return pixel_;
}

#ifdef TRACK_STATISTICS
void ProcessingElement::update_static(double time) {
    for (auto& [_, a]: analogue_registers_) {
        a->update_static(time);
    }

    for (auto& [_, d]: digital_registers_) {
        d->update_static(time);
    }

    pixel_->update_static(time);
}

cv::Mat ProcessingElement::get_transistor_count() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_32S);

    for (auto& [_, a]: analogue_registers_) {
        out += a->get_transistor_count();
    }

    for (auto& [_, d]: digital_registers_) {
        out += d->get_transistor_count();
    }

    out += pixel_->get_transistor_count();
    return out;
}

cv::Mat ProcessingElement::get_static_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for (auto& [_, a]: analogue_registers_) {
        out += a->get_static_energy();
    }
    for (auto& [_, d]: digital_registers_) {
        out += d->get_static_energy();
    }
    out += pixel_->get_static_energy();
    return out;
}

cv::Mat ProcessingElement::get_dynamic_energy() {
    cv::Mat out = cv::Mat::zeros(rows_, cols_, CV_64F);
    for (auto& [_, a]: analogue_registers_) {
        out += a->get_dynamic_energy();
    }
    for (auto& [_, d]: digital_registers_) {
        out += d->get_dynamic_energy();
    }
    out += pixel_->get_dynamic_energy();
    return out;
}

void ProcessingElement::print_stats(const CycleCounter& counter) {
    std::cout << "===================="
              << "\n";
    for (auto& [_, a]: analogue_registers_) {
        a->print_stats(counter);
    }
    for (auto& [_, d]: digital_registers_) {
        d->print_stats(counter);
    }
    pixel_->print_stats(counter);
    std::cout << "===================="
              << "\n";
}

int ProcessingElement::get_cycle_count() {
    return 0;
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
        .constructor()
        .method("analogue_registers_converter", &ProcessingElement::analogue_registers_converter)
        .method("digital_registers_converter", &ProcessingElement::digital_registers_converter)
        .method("pixel_converter", &ProcessingElement::pixel_converter)
        .method("set_rows", &ProcessingElement::set_rows)
        .method("set_cols", &ProcessingElement::set_cols)
        .method("set_row_stride", &ProcessingElement::set_row_stride)
        .method("set_col_stride", &ProcessingElement::set_col_stride)
        .method("set_analogue_registers", &ProcessingElement::set_analogue_registers)
        .method("set_digital_registers", &ProcessingElement::set_digital_registers)
        .method("set_pixel", &ProcessingElement::set_pixel)
        .method("set_config", &ProcessingElement::set_config);
};
