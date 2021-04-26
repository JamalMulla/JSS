//
// Created by jm1417 on 24/04/2021.
//

#include "simulator/external/instruction_parser.h"

#include <rttr/enumeration.h>

#include <algorithm>
#include <any>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

rttr::variant InstructionParser::get_arg(rttr::instance class_obj, std::vector<rttr::enumeration>& enums, const std::string& arg) {
    // Try to get/convert the argument. If nothing else works then will be treated as string
    // Tried in this order:
    // 1. Class property that has been registered
    // 2. Enum value of one of the given enum types
    // 3. Integer
    // 4. Float
    // 5. String
    rttr::type class_type = class_obj.get_type();
    rttr::variant arg_val = class_type.get_property_value(arg, class_obj);
    if (arg_val.is_valid()) {
        return arg_val;
    }

    for (rttr::enumeration& e: enums) {
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

rttr::method InstructionParser::get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr) {
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
            if (m.get_parameter_infos().size() > arg_types.size()) {
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

    std::cerr << "Method \"" << instr << "\" not found. It may not have been registered." << std::endl;
    exit(EXIT_FAILURE);
}

std::vector<rttr::enumeration> InstructionParser::get_enums() {
    std::vector<rttr::enumeration> enums;

    const auto& types = rttr::type::get_types();
    for (auto& type: types) {
        if (type.is_enumeration()) {
            enums.push_back(type.get_enumeration());
        }
    }

    return enums;
}

Instructions InstructionParser::parse(rttr::instance class_obj, std::ifstream& program) {

    rttr::type class_type = class_obj.get_type();
    if (!class_type.is_valid()) {
        std::cerr << "Could not find class type \"" << class_type.get_name() << "\"" << std::endl;
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

        // read the node number
        std::stringstream ss(line);
        std::string instr;
        ss >> instr;

        // read the args until end of line
        std::vector<rttr::variant> args;
        std::vector<rttr::type> arg_types;  // used for selecting the right overloaded method
        std::string arg;

        while (arg_stream >> arg) {
            rttr::variant arg_val = get_arg(class_obj, enums, arg);
            args.emplace_back(arg_val);
            arg_types.push_back(arg_val.get_type());
        }

        rttr::method instr_method = get_method(class_type, args, arg_types, instr);
        instructionArgs.emplace_back(instr_method, args);
    }
    return instructionArgs;
}

void InstructionParser::execute(Instructions parsed, rttr::instance instance) {
    for (auto& instr: parsed) {
        rttr::method& method = instr.first;
        std::vector<rttr::variant>& args = instr.second;
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
