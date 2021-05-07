//
// Created by jm1417 on 28/01/2021.
//

#include "simulator/base/architecture.h"
#include <rttr/registration>

#include <utility>


std::shared_ptr<Component> Architecture::get_component(const std::string& name) {
    return components_[name];
}

void Architecture::add_component(const std::string& name, std::shared_ptr<Component> component) {
    components_[name] = std::move(component);
}

void Architecture::add_components(std::unordered_map<std::string, std::shared_ptr<Component>> components) {
    components_.merge(components);
}

rttr::variant Architecture::components_converter(json& j) {
    std::unordered_map<std::string, std::shared_ptr<Component>> components;
    std::string component;
    try {
        for (auto& [_, value] : j.items()) {
            if (!value.contains("_name")) {
                std::cerr << "Component does not have a \"_name\" field set" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string name = value["_name"];
            if (!value.contains("_component")) {
                std::cerr << "Component does not have a \"_component\" field set" << std::endl;
                exit(EXIT_FAILURE);
            }
            component = value["_component"];
            std::shared_ptr<Component> instance = Parser::get_instance().create_instance(component, value).get_value<std::shared_ptr<Component>>();
            components[name] = instance;
        }
        return rttr::variant(components);
    } catch (json::type_error&) {
        std::cerr << "[Error] Could not parse JSON for component \"" << component << "\"" << std::endl;
    } catch (json::parse_error&) {
        std::cerr << "[Error] Could not parse JSON for component \"" << component << "\"" << std::endl;
    }

    return rttr::variant();
}

void Architecture::update_cycles(int cycles) {
#ifdef TRACK_STATISTICS
    counter_ += cycles;
#endif
}

#ifdef TRACK_STATISTICS

void Architecture::update_static() {
    double time = (1.0 / config_->get_clock_rate()) * counter_.get_cycles();
    for (auto& [_, component] : components_) {
        component->update_static(time);
    }
}

void Architecture::print_stats(int rows, int cols) {

    std::cout << "=============================="
              << "\n";
    std::cout << "Results"
              << "\n";
    std::cout << "=============================="
              << "\n";
    std::cout << "Clock rate: " << config_->get_clock_rate() << " Hz\n";
    std::cout << "Process node: " << config_->get_process_node() << " nm\n";
    std::cout << "Voltage: " << config_->get_voltage() << " V\n";
    std::cout << "Temperature: " << config_->get_temperature() << " C\n";
    std::cout << "=============================="
              << "\n";

    std::cout << "Cycle count: " << counter_.get_cycles() << "\n";
    double exec_time = ((double)counter_.get_cycles() / config_->get_clock_rate());
    std::cout << "Device execution time: " << exec_time << " s\n";
//    std::cout << "FPS: " << 500/exec_time << " \n";

    long transistor_count = 0;
    for (auto& [_, component] : components_) {
        transistor_count += cv::sum(component->get_transistor_count())[0];
    }
    std::cout << "Architecture transistor count: " << transistor_count << "\n";
    std::cout << "Average transistor count per PE: " << transistor_count / (rows * cols) << " \n";

    double static_energy = 0.0;
    for (auto& [_, component] : components_) {
        static_energy += cv::sum(component->get_static_energy())[0];
    }
    double static_power = static_energy/exec_time;

    double dynamic_energy = 0.0;
    for (auto& [_, component] : components_) {
        dynamic_energy += cv::sum(component->get_dynamic_energy())[0];
    }
    double dynamic_power = dynamic_energy/exec_time;

    std::cout << "Architecture static energy: " << static_energy << " J\n";
    std::cout << "Architecture static power: " << static_power << " W\n";
    std::cout << "Architecture dynamic energy: " << dynamic_energy << " J\n";
    std::cout << "Architecture dynamic power: " << dynamic_power << " W\n";
    std::cout << "Architecture total power: " << static_power + dynamic_power << " W\n";

//    this->pe.print_stats(counter_);
}

unsigned long long Architecture::get_cycles() {
    return counter_.get_cycles();
}
//void Architecture::print_stats(const CycleCounter& counter) {
//    this->pe.print_stats(counter);
//}
//
//void Architecture::write_stats(const CycleCounter& counter, json& j) {
//    this->pe.write_stats(counter, j);
//}
#endif

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Architecture>("Architecture")
        .method("components_converter", &Architecture::components_converter);

};
