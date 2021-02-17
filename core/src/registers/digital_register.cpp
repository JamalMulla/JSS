//
// Created by jm1417 on 28/01/2021.
//

#include <iostream>
#include "simulator/registers/digital_register.h"
#include "simulator/metrics/stats.h"

DigitalRegister::DigitalRegister(int rows, int columns, const MemoryType &memory_type)
        : Register(rows, columns, CV_8U, memory_type) {}

DigitalRegister &DigitalRegister::operator()(const std::string &name) {
    this->name_ = name;
    return *this;
}

Data DigitalRegister::read() {
    this->inc_read();
    return this->value();
}

void DigitalRegister::write(Data data) {
    this->value().setTo(data);
    this->inc_write();
}

void DigitalRegister::write(int data) {
    this->value().setTo(data);
    this->inc_write();
}

void DigitalRegister::set() {
    this->write(1);
}

void DigitalRegister::clear() {
    this->write(0);
}

void DigitalRegister::print_stats(const CycleCounter &counter) {
    // power x time = energy
    // energy / time = power

    // Keep track of energy usage.
    // So each write do + write_access_time (in) * write_power_draw
    // and same for read

    std::cout << "Register: " << this->name_ << std::endl;

    std::cout << "Energy consumed by reads: " << this->get_read_energy() << " joules" << std::endl;
    std::cout << "Energy consumed by writes: " << this->get_write_energy() << " joules" << std::endl;
    std::cout << "Total energy: " << this->get_total_energy() << " joules" << std::endl;

    //convert number of cycles to seconds based off clock rate
    double runtime_in_seconds = counter.to_seconds(stats::CLOCK_RATE);


    std::cout << "Average power for reads: " << this->get_read_energy() / runtime_in_seconds << " watts" << std::endl;
    std::cout << "Average power for writes: " << this->get_write_energy() / runtime_in_seconds << " watts" << std::endl;
    std::cout << "Total average power: " << this->get_total_energy() / runtime_in_seconds << " watts" << std::endl;


}

void DigitalRegister::write_stats(const CycleCounter &counter, json &j) {
    double runtime = counter.to_seconds(stats::CLOCK_RATE);
    auto reg_stats = json::object();
    reg_stats[this->name_] =
            {{"Energy (J)",
                     {
                             {"Reads", this->get_read_energy()},
                             {"Writes", this->get_write_energy()},
                             {"Total", this->get_total_energy()},
                     }},
             {"Power (W)",
                     {
                             {"Reads", this->get_read_energy() / runtime},
                             {"Writes", this->get_write_energy() / runtime},
                             {"Total", this->get_total_energy() / runtime},
                     }
             },
             {"Accesses",
                     {
                             {"Reads", this->get_reads()},
                             {"Writes", this->get_writes()}
                     }

             }
            };
    j.push_back(reg_stats);
}



