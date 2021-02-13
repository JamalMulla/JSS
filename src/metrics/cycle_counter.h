//
// Created by jm1417 on 13/02/2021.
//

#ifndef SIMULATOR_CYCLE_COUNTER_H
#define SIMULATOR_CYCLE_COUNTER_H

#include <cstdint>
#include <iostream>

class CycleCounter {
private:
    unsigned long long counter = 0;

public:
    CycleCounter& operator+=(int cycles);
    CycleCounter& operator++();                  // Prefix
    const CycleCounter  operator++(int cycles);  // Postfix

    CycleCounter& operator-=(int cycles);
    CycleCounter& operator--();                  // Prefix
    const CycleCounter operator--(int cycles);   // Postfix

    double to_seconds(long clock_rate);

    friend std::ostream& operator<<(std::ostream& os, const CycleCounter& cc);

};




#endif //SIMULATOR_CYCLE_COUNTER_H
