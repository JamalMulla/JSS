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
    static rttr::variant get_arg(const std::string& s);
    static std::vector<std::pair<rttr::method, std::vector<rttr::variant> > > parse(const rttr::type& class_type, rttr::instance obj, const std::string& program_file);

};

#endif  //SIMULATOR_INSTRUCTION_PARSER_H
