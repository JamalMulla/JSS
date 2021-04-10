//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H

#include <nlohmann/json.hpp>

#include "simulator/metrics/cycle_counter.h"

using json = nlohmann::json;
class Component {
   public:
#ifdef TRACK_STATISTICS
    /* Returns the amount of static power dissipation in the amount of time passed (in seconds).*/
    virtual double get_static_power(double time) = 0;
    /* Returns the amount of dynamic power dissipation.
     * The operation is executed first and the amount of dynamic power is stored.
     * This is then returned when this method is called. */
    virtual double get_dynamic_power() = 0;
    /* Simply the number of transistors needed for the component.
     * This should be a constant after the component is configured. */
    virtual int get_transistor_count() = 0;
    /* The number of cycles needed for the operation.
     * Similar to dynamic power as the value is stored after an operation*/
    virtual int get_cycle_count() = 0;
//    virtual void print_stats(const CycleCounter& counter) = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
#endif
};

#endif  // SIMULATOR_COMPONENT_H
