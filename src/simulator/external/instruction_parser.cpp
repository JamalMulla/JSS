//
// Created by jm1417 on 24/04/2021.
//

#include "simulator/external/instruction_parser.h"

#include <algorithm>
#include <any>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../../../../.conan/data/rttr/0.9.6/_/_/package/b173bbda18164d49a449ffadc1c9e817f49e819d/include/rttr/enumeration.h"

std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > InstructionParser::parse(const rttr::type& class_type, rttr::instance obj, const std::string& program_file) {
    std::ifstream file(program_file);

    if(!file.is_open()) {
        std::cerr << "Error opening file " << program_file << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > instructionArgs;

    // read until you reach the end of the file

    std::string line;
    while(std::getline(file, line)) {
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
        std::string arg;

        // todo add some flexiblity here. maybe a list of types to check
        rttr::type enum_type = rttr::type::get_by_name("news_t");
        if (!enum_type || !enum_type.is_enumeration())
        {
            std::cout << "No enum for news_t found" << std::endl;
        }
        rttr::enumeration enum_news = enum_type.get_enumeration();

        while(arg_stream >> arg) {

            rttr::variant arg_val = class_type.get_property_value(arg, obj);
            if (!arg_val.is_valid()) {
                arg_val = enum_news.name_to_value(arg);
            }

            args.emplace_back(arg_val);
        }

        instructionArgs.emplace_back(class_type.get_method(instr), args);
    }
    return instructionArgs;
}
