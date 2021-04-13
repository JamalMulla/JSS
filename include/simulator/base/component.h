//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H

#include <nlohmann/json.hpp>
#include <opencv2/core.hpp>

#include "simulator/metrics/stats_outputter.h"
#include "simulator/metrics/cycle_counter.h"

//using json = nlohmann::json;
class Component : public StatsOutputter {
   public:
#ifdef TRACK_STATISTICS
    /* Update component with how much time has passed for operation. Used for updating static power. */
    virtual void update(double time) = 0;
    /* Returns the total amount of static energy use */
    virtual cv::Mat& get_static_energy() = 0;
    /* Returns the total amount of dynamic energy use */
    virtual cv::Mat& get_dynamic_energy() = 0;
    /* Returns the transistor count across the array
     * This should be a constant after the component is configured. */
    virtual cv::Mat& get_transistor_count() = 0;
    /* The number of cycles needed for the operation.
     * Similar to dynamic power as the value is stored after an operation */
    virtual int get_cycle_count() = 0;
    void print_stats(const CycleCounter& counter) override = 0;
//    virtual void write_stats(const CycleCounter& counter, json& j) = 0;
#endif
};

#endif  // SIMULATOR_COMPONENT_H
