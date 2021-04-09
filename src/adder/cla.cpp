//
// Created by jm1417 on 09/04/2021.
//
#include "simulator/adders/cla.h"

/*https://www.researchgate.net/publication/283818559_A_comparative_study_of_high_speed_CMOS_adders_using_Microwind_and_FPGA
 * */

CarryLookAheadAdder::CarryLookAheadAdder(int bits, const Config& config) :
    cycle_count_(1),
    transistor_count_(fun_transistor(bits, config)),
    static_power_(fun_static(bits, config)),
    dynamic_power_(fun_dynamic(bits, config)) {}

int CarryLookAheadAdder::fun_transistor(int bits, const Config& config) {
    // Transistor count based off number of bits and config
    return 12 + 26 + ((bits - 2) * 28);
}

double CarryLookAheadAdder::fun_static(int bits, const Config& config) {
    // Static power dissipation based off number of bits and config
    return 2.0e-5;
}

double CarryLookAheadAdder::fun_dynamic(int bits, const Config& config) {
    // Dynamic power dissipation based off number of bits and config
    return 0.00015;
}

double CarryLookAheadAdder::get_static_power(double time) {
    return static_power_ * time;
}

double CarryLookAheadAdder::get_dynamic_power() {
    return dynamic_power_;
}

int CarryLookAheadAdder::get_cycle_count() {
    return cycle_count_;
}

int CarryLookAheadAdder::get_transistor_count() {
    return transistor_count_;
}

void CarryLookAheadAdder::print_stats(const CycleCounter& counter) {

}

void CarryLookAheadAdder::write_stats(const CycleCounter& counter, json& j) {

}

void CarryLookAheadAdder::add(DigitalRegister& dst, const DigitalRegister& src1, const DigitalRegister& src2) {
    dst.value() = src1.value() + src2.value();
}

