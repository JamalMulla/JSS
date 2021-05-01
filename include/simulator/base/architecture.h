//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_ARCHITECTURE_H
#define SIMULATOR_ARCHITECTURE_H

#include <simulator/adders/cla.h>
#include <simulator/memory/dram_array.h>
#include <simulator/metrics/stats_outputter.h>

#include <functional>

#include "component.h"
#include "plane_params.h"
#include "processing_element.h"

// TODO implement StatsOutputter
class Architecture {
   private:
    CycleCounter counter_;
    Config config_;

   public:
    std::unordered_map<std::string, std::shared_ptr<Component>> components;

    std::shared_ptr<Component> get_component(const std::string& name);

    template<class type>
    std::shared_ptr<type> get_component(const std::string& name) {
        return components[name];
    }

    void add_component(const std::string& name, std::shared_ptr<Component> component);

    void update_cycles(int cycles);
#ifdef TRACK_STATISTICS
    void update_static(); // Calculates static power using the number of cycles we've got to
    void print_stats();
    unsigned long long get_cycles();
#endif
};

#endif  // SIMULATOR_ARCHITECTURE_H
