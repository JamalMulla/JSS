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

   protected:
    std::shared_ptr<Config> config_;

   public:
    std::unordered_map<std::string, std::shared_ptr<Component> > components_;

    template<class type>
    std::shared_ptr<type> get_component(const std::string& name) {
        return std::dynamic_pointer_cast<type>(components_[name]);
    }

    void add_component(const std::string& name, std::shared_ptr<Component> component);
    void add_components(std::unordered_map<std::string, std::shared_ptr<Component> > components);

    void update_cycles(int cycles);
#ifdef TRACK_STATISTICS
    void update_static(); // Calculates static power using the number of cycles we've got to
    void print_stats();
    unsigned long long get_cycles();
#endif
    std::shared_ptr<Component> get_component(const std::string& name);
};

#endif  // SIMULATOR_ARCHITECTURE_H
