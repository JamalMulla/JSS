//
// Created by jm1417 on 14/02/2021.
//

#ifndef SIMULATOR_JSON_WRITER_H
#define SIMULATOR_JSON_WRITER_H

#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;
class JsonWriter {
   public:
    json j;
};

#endif  // SIMULATOR_JSON_WRITER_H
