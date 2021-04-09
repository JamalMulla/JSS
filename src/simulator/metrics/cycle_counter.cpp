//
// Created by jm1417 on 13/02/2021.
//

#include "simulator/metrics/cycle_counter.h"

CycleCounter &CycleCounter::operator+=(int cycles) {
    this->counter += cycles;
    return *this;
}

CycleCounter &CycleCounter::operator++() {
    this->counter++;
    return *this;
}

const CycleCounter CycleCounter::operator++(int cycles) {
    CycleCounter ret = *this;
    this->operator++();
    return ret;
}

CycleCounter &CycleCounter::operator-=(int cycles) {
    this->counter -= cycles;
    return *this;
}

CycleCounter &CycleCounter::operator--() {
    this->counter--;
    return *this;
}

const CycleCounter CycleCounter::operator--(int cycles) {
    CycleCounter ret = *this;
    this->operator--();
    return ret;
}

double CycleCounter::to_seconds(long clock_rate) const {
    return static_cast<double>(this->counter) / clock_rate;
}

long CycleCounter::to_cycles(double seconds, long clock_rate) const {
    return seconds * clock_rate;
}

std::ostream &operator<<(std::ostream &os, const CycleCounter &cc) {
    os << cc.counter;
    return os;
}

unsigned long long CycleCounter::get_cycles() const { return this->counter; }
