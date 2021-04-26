//
// Created by jm1417 on 24/04/2021.
//

#ifndef SIMULATOR_INSTRUCTION_PARSER_H
#define SIMULATOR_INSTRUCTION_PARSER_H

#include <rttr/variant.h>

#include <iostream>
#include <string>
#include <vector>

typedef std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > Instructions;

class InstructionParser {
   public:
    static rttr::variant get_arg(rttr::instance class_obj, std::vector<rttr::enumeration>& enums, const std::string& arg);
    static rttr::method get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr);
    static std::vector<rttr::enumeration> get_enums();
    static Instructions parse(rttr::instance class_obj, std::ifstream& program);
    static void execute(Instructions parsed, rttr::instance instance);
};

#endif  //SIMULATOR_INSTRUCTION_PARSER_H
