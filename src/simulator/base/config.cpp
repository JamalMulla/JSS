//
// Created by jm1417 on 12/04/2021.
//
#include "simulator/base/config.h"

Config::Config(long clock_rate, int process_node, double voltage, int temperature) :
    clock_rate(clock_rate),
    process_node(process_node),
    voltage(voltage),
    temperature(temperature) {}
