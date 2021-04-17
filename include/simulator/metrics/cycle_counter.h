//
// Created by jm1417 on 13/02/2021.
//

#ifndef SIMULATOR_CYCLE_COUNTER_H
#define SIMULATOR_CYCLE_COUNTER_H

#include <iostream>

class CycleCounter {
   private:
    unsigned long long counter = 0;

   public:
    CycleCounter& operator+=(int cycles);
    CycleCounter& operator++();                 // Prefix
    const CycleCounter operator++(int cycles);  // Postfix

    CycleCounter& operator-=(int cycles);
    CycleCounter& operator--();                 // Prefix
    const CycleCounter operator--(int cycles);  // Postfix

    [[nodiscard]] unsigned long long get_cycles() const;

    [[nodiscard]] double to_seconds(long clock_rate) const;
    [[nodiscard]] long to_cycles(double seconds, long clock_rate) const;

    friend std::ostream& operator<<(std::ostream& os, const CycleCounter& cc);
};

#endif  // SIMULATOR_CYCLE_COUNTER_H
