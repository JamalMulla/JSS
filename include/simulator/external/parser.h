//
// Created by jm1417 on 24/04/2021.
//

#ifndef SIMULATOR_PARSER_H
#define SIMULATOR_PARSER_H

#include <rttr/variant.h>

#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > Instructions;

namespace Parser {
    rttr::variant get_arg(rttr::instance class_obj, std::vector<rttr::enumeration>& enums, const std::string& arg);
    rttr::method get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr);
    std::vector<rttr::enumeration> get_enums();
    Instructions parse_instructions(rttr::instance class_obj, std::ifstream& program);
    void execute_instructions(Instructions parsed, rttr::instance instance);
    void parse_config(std::ifstream& config);
};

#endif  //SIMULATOR_PARSER_H
