//
// Created by jm1417 on 26/04/2021.
//

#ifndef SIMULATOR_CONFIG_PARSER_H
#define SIMULATOR_CONFIG_PARSER_H

#include <fstream>
class ConfigParser {
   public:
    static void parse_config(std::ifstream& config);

};

#endif  //SIMULATOR_CONFIG_PARSER_H
