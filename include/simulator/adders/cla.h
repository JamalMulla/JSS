//
// Created by jm1417 on 09/04/2021.
//

#ifndef SIMULATOR_CLA_H
#define SIMULATOR_CLA_H

#include <simulator/registers/digital_register.h>

#include "simulator/base/component.h"
#include "simulator/base/config.h"

class CarryLookAheadAdder : public Component {
   private:
    int cycle_count_;
    int transistor_count_;
    double static_power_; // in Watts
    double dynamic_power_; // in Watts for an addition
    int fun_transistor(int bits, const Config& config);
    double fun_static(int bits, const Config& config);
    double fun_dynamic(int bits, const Config& config);

   public:
    CarryLookAheadAdder(int bits, const Config& config);
    double get_static_power(double time) override;
    double get_dynamic_power() override;
    int get_cycle_count() override;
    int get_transistor_count() override;
    void print_stats(const CycleCounter &counter) override;
    void write_stats(const CycleCounter &counter, json &j) override;

    void add(DigitalRegister& dst, const DigitalRegister& src1, const DigitalRegister& src2);

};


#endif  //SIMULATOR_CLA_H

