//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

class Memory : public Component {
   public:
    Memory() = default;
    double get_static_power(double time) override = 0;
    double get_dynamic_power() override = 0;
    int get_cycle_count() override = 0;
    int get_transistor_count() override = 0;
};

#endif  // SIMULATOR_MEMORY_H
