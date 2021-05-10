//
// Created by jm1417 on 07/05/2021.
//

#ifndef SIMULATOR_ADC_H
#define SIMULATOR_ADC_H

#include "simulator/base/component.h"
#include "simulator/base/config.h"

class ADC : public Component {
    RTTR_ENABLE(Component);
   private:
    double time_;

   public:
    ADC() = default;
    void init();

    int convert(int analogue);

    void update_dynamic(int count);

#ifdef TRACK_STATISTICS
    int calc_transistor_count() override;
    double calc_static() override;
    double calc_dynamic() override;

    void update_static(double time) override;
    int get_cycle_count() override;
    void print_stats(const CycleCounter &counter) override;
#endif
};

#endif  //SIMULATOR_ADC_H
