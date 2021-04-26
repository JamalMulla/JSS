//
// Created by jm1417 on 24/04/2021.
//

#ifndef SIMULATOR_INSTRUCTION_PARSER_H
#define SIMULATOR_INSTRUCTION_PARSER_H

#include <rttr/variant.h>

#include <string>
#include <vector>

class InstructionParser {
   public:
    static rttr::variant get_arg(const rttr::type& class_type, rttr::instance class_obj, std::vector<rttr::enumeration>& enums, const std::string& arg);
    static rttr::method get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr);
    static std::vector<rttr::enumeration> get_enums(std::vector<std::string>& type_names);
    static std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > parse(const rttr::type& class_type, rttr::instance class_obj, std::vector<std::string>& type_names, const std::string& program_file);

};

#endif  //SIMULATOR_INSTRUCTION_PARSER_H
