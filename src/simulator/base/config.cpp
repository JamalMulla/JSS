//
// Created by jm1417 on 12/04/2021.
//
#include "simulator/base/config.h"

#include <rttr/registration>

Config::Config(long clock_rate, int process_node, double voltage, int temperature) :
    clock_rate_(clock_rate),
    process_node_(process_node),
    voltage_(voltage),
    temperature_(temperature) {}

void Config::set_clock_rate(long clock_rate) {
    this->clock_rate_ = clock_rate;
}

void Config::set_process_node(int process_node) {
    this->process_node_ = process_node;
}

void Config::set_voltage(double voltage) {
    this->voltage_ = voltage;
}

void Config::set_temperature(int temperature) {
    this->temperature_ = temperature;
}

RTTR_REGISTRATION {
    using namespace rttr;

    registration::class_<Config>("Config")
        .constructor<long, int, double, int>()(default_arguments((long)1e6, 180, 1.8, 20))
        .method("set_clock_rate", &Config::set_clock_rate)
        .method("set_process_node", &Config::set_process_node)
        .method("set_voltage", &Config::set_voltage)
        .method("set_temperature", &Config::set_temperature);
};
