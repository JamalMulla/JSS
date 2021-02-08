//
// Created by jm1417 on 08/02/2021.
//

#include "component.h"

void Component::set_clock_speed(long clock_speed) {
    this->clock_speed = clock_speed;
}

long Component::get_clock_speed() {
    return this->clock_speed;
}
