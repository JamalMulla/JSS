//
// Created by jm1417 on 24/04/2021.
//

#include "simulator/external/parser.h"

#include <opencv2/core/ocl.hpp>
#include <rttr/enumeration.h>
#include <rttr/type.h>
#include <simulator/registers/analogue_register.h>
#include <simulator/ui/ui.h>

#include <algorithm>
#include <any>
#include <fstream>
#include <iostream>
#include <rttr/registration>
#include <sstream>
#include <utility>
#include <vector>

Parser::Parser() {
    enums_ = get_enums();
}

static inline const rttr::type& check_validity(const rttr::type& type, const std::string& name) {
    if (!type.is_valid()) {
        std::cerr << "Could not find \"" << name << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    return type;
}

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

rttr::variant Parser::get_arg(rttr::instance class_obj, const std::string& arg) {
    // Try to get/convert the argument. If nothing else works then will be treated as string
    // Tried in this order:
    // 1. Class property that has been registered
    // 2. Cache
    // 3. Enum value of one of the given enum types
    // 4. Integer
    // 5. Float
    // 6. String
    rttr::type class_type = class_obj.get_type();

    rttr::variant arg_val = class_type.get_property_value(arg, class_obj);

    if (arg_val.is_valid()) {
        return arg_val;
    }

    if (cache.find(arg) != cache.end()) {
        // found in cache
        return cache[arg];
    }

    for (rttr::enumeration& e: enums_) {
        arg_val = e.name_to_value(arg);
        if (arg_val.is_valid()) {
            return arg_val;
        }
    }

    char* p = nullptr;
    auto i = std::strtol(arg.data(), &p, 10);
    if (p == arg.data() + arg.size()) {
        // Successfully converted to int
        return rttr::variant(int(i));
    }

    auto f = std::strtof(arg.data(), &p);
    if (p == arg.data() + arg.size()) {
        // Successfully converted to float
        return rttr::variant(f);
    }

    return rttr::variant(arg);
}

rttr::method Parser::get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr) {
    // TODO remove redundancy
    rttr::method instr_method = class_type.get_method(instr, arg_types);

    if (instr_method.is_valid()) {
        // Go through and convert each argument to the proper type.
        // This is needed as auto casting is not done on method invocation
        size_t i = 0;
        for (auto& p: instr_method.get_parameter_infos()) {
            if (i >= args.size()) {
                break;
            }
            if (p.get_type().is_arithmetic()) {
                args.at(i).convert(p.get_type());
            }
            i++;
        }
        return instr_method;
    }

    // Possible that the method exists but has default arguments which have not been passed
    // Need to search through for a method that is likely the right one

    for (auto& m: class_type.get_methods()) {
        if (m.get_name() == instr) {
            if (m.get_parameter_infos().size() >= arg_types.size()) {
                // at least as many arguments as has been given
                // assume this is the correct one
                // may not actually be the correct one which is a limitation.
                size_t i = 0;
                for (auto& p: m.get_parameter_infos()) {
                    if (i >= args.size()) {
                        break;
                    }
                    if (p.get_type().is_arithmetic()) {
                        args.at(i).convert(p.get_type());
                    }
                    i++;
                }
                return m;
            }
        }
    }

    std::cerr << "Method \"" << instr << "\" not found with given argument types. It may not have been registered or you may have passed arguments with incorrect types. Continuing" << std::endl;
    return instr_method;
//    exit(EXIT_FAILURE);
}

std::vector<rttr::enumeration> Parser::get_enums() {
    std::vector<rttr::enumeration> enums;

    const auto& types = rttr::type::get_types();
    for (auto& type: types) {
        if (type.is_enumeration()) {
            enums.push_back(type.get_enumeration());
        }
    }

    return enums;
}

Instructions Parser::parse_instructions(rttr::instance class_obj, std::ifstream& program) {
    rttr::type class_type = class_obj.get_wrapped_instance().get_type(); // assuming arch will be shared_ptr<Class>
    if (!class_type.is_valid()) {
        std::cerr << "Could not find class type" << std::endl;
        exit(EXIT_FAILURE);
    }

    Instructions instructionArgs;
    std::vector<rttr::enumeration> enums = get_enums();

    // read until you reach the end of the file
    std::string line;
    while (std::getline(program, line)) {
        ltrim(line);
        if (line.empty() || line.rfind("//", 0) == 0) {
            continue;
        }
        unsigned first = line.find('(');
        unsigned last = line.find(')');
        std::string arg_string = line.substr(first + 1, last - first - 1);

        //replace commas with space
        std::replace(arg_string.begin(), arg_string.end(), ',', ' ');
        std::stringstream arg_stream(arg_string);

        std::replace(line.begin(), line.end(), '(', ' ');
        std::replace(line.begin(), line.end(), ')', ' ');

        std::stringstream ss(line);
        std::string instr;
        ss >> instr;

        // read the args until end of line
        std::vector<rttr::variant> args;
        std::vector<rttr::type> arg_types;  // used for selecting the right overloaded method
        std::string arg;

        while (arg_stream >> arg) {
            rttr::variant arg_val = get_arg(class_obj, arg);
            args.emplace_back(arg_val);
            arg_types.push_back(arg_val.get_type());
        }

        rttr::method instr_method = get_method(class_type, args, arg_types, instr);
        if (instr_method.is_valid()) {
            instructionArgs.emplace_back(instr_method, args);
        }
    }
    return instructionArgs;
}

void Parser::execute_instructions(const Instructions& parsed, rttr::instance instance) {
    for (auto& [method, args]: parsed) {
        rttr::variant res;
        switch (args.size()) {
            case 0: res = method.invoke(instance); break;
            case 1: res = method.invoke(instance, args[0]); break;
            case 2: res = method.invoke(instance, args[0], args[1]); break;
            case 3: res = method.invoke(instance, args[0], args[1], args[2]); break;
            case 4: res = method.invoke(instance, args[0], args[1], args[2], args[3]); break;
            case 5: res = method.invoke(instance, args[0], args[1], args[2], args[3], args[4]); break;
            case 6: res = method.invoke(instance, args[0], args[1], args[2], args[3], args[4], args[5]); break;
            default: std::cerr << "Too many arguments in method " << method.get_name() << " invocation" << std::endl;
        }
        if (!res.is_valid()) {
            std::cerr << "Could not execute method \"" << method.get_name() << "\""
                      << " with " << args.size() << " arguments" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void Parser::set_property(const rttr::type& arch_type, const rttr::variant& arch, const std::string& name, rttr::variant value) {
    rttr::method prop_method = arch_type.get_method("set_" + name);
    if (!prop_method.is_valid()) {
        std::cerr << "Could not find setter for property \"" << name << "\" of object " << arch_type.get_name() << std::endl;
        exit(EXIT_FAILURE);
    }

    bool converted = value.convert(prop_method.get_parameter_infos().begin()->get_type());  //only 1 parameter for each prop setter
    if (!converted) {
        std::cerr << "Could not convert property \"" << name << "\" of type " << value.get_type().get_name() << " to required type " << prop_method.get_parameter_infos().begin()->get_type().get_name() << std::endl;
        exit(EXIT_FAILURE);
    }

    if (!prop_method.invoke(arch, value).is_valid()) {
        std::cerr << "Could not call method \"" << prop_method.get_name() << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
}

rttr::variant Parser::create_instance(const std::string& arch_name, json arch_props) {
    rttr::type class_type = check_validity(rttr::type::get_by_name(arch_name), arch_name);

    std::cout << "Creating instance of " << arch_name << std::endl;
    // Create instance of class_obj type
    rttr::variant class_obj = class_type.create();
    if (!class_obj.is_valid()) {
        std::cerr << "Could not create architecture \"" << arch_name << "\" using viable constructor" << std::endl;
        exit(EXIT_FAILURE);
    }

    // check if there are any properties to inherit.
    // If there are then find them from the store
    std::vector<std::string> to_inherit;
    if (arch_props.contains("_inherit")) {
        for (auto& [_, name] : arch_props["_inherit"].items()) {
            auto val = cache.find(name);
            if (val == cache.end()) {
                // not found in cache
                std::cerr << "Could not inherit property " << name << " as it does not exist in the cache" << std::endl;
                exit(EXIT_FAILURE);
            }
//            std::cout << arch_name << "->" << name << "=" << val->second.convert<std::string>() << std::endl;
            set_property(class_type, class_obj, name, val->second);
        }
    }

    // If architecture properties are defined, they are used as constructor arguments
    // matching constructor must exist
    if (arch_props.is_object()) {
        for (auto& [key, value]: arch_props.items()) {
            // Skip keywords todo improve
            if (key == "_inherit") { continue; }
            if (key == "_name") { continue; }
            if (key == "_component") { continue; }

            rttr::variant val;
            rttr::method prop_converter = class_type.get_method(key + "_converter");
            if (prop_converter.is_valid()) {
                std::cout << "Found converter for property \"" << key << "\"" << std::endl;
                if (prop_converter.get_parameter_infos().size() == 1) {
                    val = prop_converter.invoke(class_obj, (json) value);
                } else if (prop_converter.get_parameter_infos().size() == 2) {
                    val = prop_converter.invoke(class_obj, (json) value, cache);
                } else {
                    std::cerr << "Invalid number of arguments in converter for property \"" << key << "\". Expected 1 or 2 but got " << prop_converter.get_parameter_infos().size() << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                // No converter found
                if (value.is_string()) {
                    val = get_arg({}, value);
                } else if (value.is_boolean()) {
                    val = rttr::argument(value.get<bool>());
                } else if (value.is_number_float()) {
                    val = rttr::variant(value.get<float>());
                } else if (value.is_number_integer()) {
                    val = rttr::variant(value.get<int>());
                } else if (value.is_number_unsigned()) {
                    val = rttr::variant(value.get<uint>());
                }
            }
            if (!val.is_valid()) {
                std::cerr << "Could not parse property \"" << key << "\" of type \"" << value.type_name() << "\"" << std::endl;
                exit(EXIT_FAILURE);
            }

            // add to cache
            cache[key] = val;

            set_property(class_type, class_obj, key, val);
        }
    }

    // if an init() method exists then call it
    rttr::method init = class_type.get_method("init");
    if (init.is_valid()) {
        init.invoke(class_obj);
    }

    return class_obj;
}

void Parser::parse_config(std::ifstream& config, std::ifstream& program) {
    json c = json::parse(config);

    setup_processing(c);

    std::vector<rttr::enumeration> enums = get_enums();  // all registered enums

    // Create architecture by using builder
    std::string arch_name = c["architecture"];
    json arch_props = c[arch_name];
    rttr::variant arch = create_instance(arch_name, arch_props);
    if (arch_props.empty()) {
        std::cerr << "No architecture definition found for \"" << arch_name << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }

    // UI
    bool ui_enabled = false;
    if (c.contains("ui_enabled")) {
        ui_enabled = c["ui_enabled"];
    }

    std::vector<std::shared_ptr<Register>> regs_to_display;
    UI& ui = UI::get_instance();
    if (ui_enabled) {
        if (c.contains("ui_registers_to_display")) {
            json regs = c["ui_registers_to_display"];
            for (const json& reg: regs) {
                rttr::variant arg = get_arg(arch, reg.get<std::string>());
                if (!arg.is_valid()) {
                    std::cerr << "Could not get register " << reg.get<std::string>() << std::endl;
                    exit(EXIT_FAILURE);
                }
                regs_to_display.push_back(arg.get_value<std::shared_ptr<Register> >());
            }
        }
    }

    Instructions instructions = Parser::parse_instructions(arch, program);

    // timing
    bool frame_time = true;
    if (c.contains("frame_time")) {
        frame_time = c["frame_time"].get<bool>();
    }

    // Iterations
    int frames = 1000;
    if (c.contains("frames")) {
        frames = c["frames"];
    }


    bool loop = frames < 0;

    int i = 0;
    while (loop || i < frames) {
        int e1 = cv::getTickCount();
        Parser::execute_instructions(instructions, arch);

        if (frame_time) {
            int e2 = cv::getTickCount();
            std::cout << ((double)(e2 - e1) / cv::getTickFrequency()) * 1000 << " ms" << std::endl;
        }

        if (ui_enabled) {
            for (auto& reg: regs_to_display) {
                ui.display_reg(reg);
            }
        }

        if (!loop) {
            if (i % 50 == 0) {
                std::cout << "Frame: " << i << "/" << frames << std::endl;
            }
        } else {
            if (i % 100 == 0) {
                std::cout << "Frame: " << i << std::endl;
            }
        }
        i++;

    }

    // Stats
    if (c.contains("with_stats") && c["with_stats"] == true) {
        std::vector<rttr::argument> args;
        args.emplace_back(c);
        if (c.contains("output_filename")){
            args.emplace_back(c["output_filename"].get<std::string>());
        }
        if (!arch.extract_wrapped_value().get_type().invoke("print_stats", arch, args).is_valid()) {
            std::cerr << "Could not print stats for run. Has a \"print_stats\" method been registered?" << std::endl;
        }
    }

    // Clean up and call destructors.
//    arch_builder_type.destroy(arch_builder);
    arch.get_type().destroy(arch);
}


void Parser::setup_processing(json& j) {

    bool use_opencl = false;
    if (j.contains("use_opencl")) {
        use_opencl = j["use_opencl"].get<bool>();
    }

    if (!use_opencl) {
        std::cout << "Processing on CPU" << std::endl;
        cv::ocl::setUseOpenCL(false);
    } else {
        cv::ocl::setUseOpenCL(true);

        if (!cv::ocl::haveOpenCL())
        {
            std::cout << "OpenCL is not available" << std::endl;
            return;
        }

        cv::ocl::Context context;
        if (!context.create(cv::ocl::Device::TYPE_GPU))
        {
            std::cout << "Failed to create GPU Context" << std::endl;
            //return;
        }

        std::cout << context.ndevices() << " GPU device(s) detected." << std::endl;
        for (size_t i = 0; i < context.ndevices(); i++)
        {
            cv::ocl::Device device = context.device(i);
            std::cout << "name:              " << device.name() << std::endl;
            std::cout << "available:         " << device.available() << std::endl;
            std::cout << "imageSupport:      " << device.imageSupport() << std::endl;
            std::cout << "OpenCL_C_Version:  " << device.OpenCL_C_Version() << std::endl;
            std::cout << std::endl;
        }
        std::cout << "Processing on GPU using OpenCL" << std::endl;
    }

}
