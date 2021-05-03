//
// Created by jm1417 on 24/04/2021.
//

#ifndef SIMULATOR_PARSER_H
#define SIMULATOR_PARSER_H

#include <rttr/variant.h>
#include <rttr/enumeration.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using Instructions = std::vector<std::pair<rttr::method, std::vector<rttr::variant> > >;
using ParserCache = std::unordered_map<std::string, rttr::variant>;
using json = nlohmann::ordered_json;

class Parser {
   private:
    Parser();
    std::vector<rttr::enumeration> enums_;
    ParserCache cache;

   public:
    static Parser& get_instance(){
        static Parser instance;
        return instance;
    }

    rttr::variant get_arg(rttr::instance class_obj, const std::string& arg);
    rttr::method get_method(const rttr::type& class_type, std::vector<rttr::variant>& args, std::vector<rttr::type>& arg_types, const std::string& instr);
    std::vector<rttr::enumeration> get_enums();
    Instructions parse_instructions(rttr::instance class_obj, std::ifstream& program);
    void execute_instructions(const Instructions& parsed, rttr::instance instance);
    void set_property(const rttr::type& arch_type, const rttr::variant& arch, const std::string& name, rttr::variant value);
    rttr::variant create_instance(const std::string& arch_name, json arch_props);
    void parse_config(std::ifstream& config, std::ifstream& program);
};

#endif  //SIMULATOR_PARSER_H
