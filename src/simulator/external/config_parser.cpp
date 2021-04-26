//
// Created by jm1417 on 26/04/2021.
//
#include "simulator/external/config_parser.h"

#include <rttr/type.h>

#include <iostream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

inline rttr::type& check_validity(const rttr::type& type, const std::string& name)
{
    if (!type.is_valid()) {
        std::cerr << "Could not find \"" << name << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    return const_cast<rttr::type&>(type);
}

void ConfigParser::parse_config(std::ifstream &config) {
    json c = json::parse(config);

    // Create architecture by calling default constructor
    std::string arch_name = c["architecture"];
    rttr::type arch_type = check_validity(rttr::type::get_by_name(arch_name), arch_name);
    rttr::variant arch_obj = arch_type.create();

    // If architecture properties are defined, find them and set them
    json arch_props = c[arch_name];
    if (arch_props.is_object()) {
        for (auto& json_prop: arch_props.items()) {
            rttr::property prop = arch_type.get_property(json_prop.key());
            if (!prop.is_valid()) {
                std::cerr << "Could not find property \"" << json_prop.key() << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }
            auto val = rttr::variant(json_prop.value());
            bool converted = val.convert(prop.get_type());
            if (!converted) {
                std::cerr << "Could not convert property \"" << json_prop.key() << "\" to required type " << prop.get_type().get_name() << std::endl;
                exit(EXIT_FAILURE);
            }
            prop.set_value(arch_obj, val);
        }
    }

    // UI
    bool ui_enabled = false;
    if (c.contains("ui_enabled")) {
        ui_enabled = c["ui_enabled"];
    }

    if (ui_enabled) {
        std::vector<std::string> regs_to_display;
        if (c.contains("ui_registers_to_display")) {
            json regs = c["ui_registers_to_display"];
            for (const std::string reg : regs) {
                regs_to_display.push_back(reg);
            }
        }
        std::cout << "Got " << regs_to_display.size() << " registers to display" << std::endl;
    }





    // Clean up

}
