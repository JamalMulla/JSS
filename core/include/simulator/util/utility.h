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
    void onMouse(int event, int x, int y, int, void* reg);
    int normalise(int value, int old_low, int old_high, int new_low, int new_high);

}


#endif //SIMULATOR_UTILITY_H
