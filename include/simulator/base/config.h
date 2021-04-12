//
// Created by jm1417 on 09/04/2021.
//

#ifndef SIMULATOR_CONFIG_H
#define SIMULATOR_CONFIG_H

struct Config {
    long clock_rate; // in Hz
    int process_node; // in nm
    double voltage;
    int temperature; // in degrees celsius
};

#endif  //SIMULATOR_CONFIG_H
