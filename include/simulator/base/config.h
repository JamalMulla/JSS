//
// Created by jm1417 on 09/04/2021.
//

#ifndef SIMULATOR_CONFIG_H
#define SIMULATOR_CONFIG_H

struct Config {
   private:
    long clock_rate_; // in Hz
    int process_node_; // in nm
    double voltage_;
    int temperature_; // in degrees celsius

   public:
    explicit Config(long clock_rate = 1e6, int process_node = 180, double voltage = 1.8, int temperature = 20);

    long get_clock_rate();
    int get_process_node();
    long get_voltage();
    long get_temperature();

    void set_clock_rate(long clock_rate);
    void set_process_node(int process_node);
    void set_voltage(double voltage);
    void set_temperature(int temperature);
};

#endif  //SIMULATOR_CONFIG_H
