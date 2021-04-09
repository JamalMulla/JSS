//
// Created by jm1417 on 08/02/2021.
//

#include "simulator/metrics/stats.h"

long stats::CLOCK_RATE = 0;
double stats::CYCLE_TIME = 0;

void stats::set_clock_rate(long clock_rate) {
    stats::CLOCK_RATE = clock_rate;
    stats::CYCLE_TIME = 1.0 / clock_rate;
}
