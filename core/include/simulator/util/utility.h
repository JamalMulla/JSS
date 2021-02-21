//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_UTILITY_H
#define SIMULATOR_UTILITY_H

#include <string>
#include "simulator/registers/register.h"

namespace utility {

    // Vision
    void display_register(const std::string& window_name, Register& reg);

}


#endif //SIMULATOR_UTILITY_H
