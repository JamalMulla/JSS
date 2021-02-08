//
// Created by jm1417 on 08/02/2021.
//

#ifndef SIMULATOR_COMPONENT_H
#define SIMULATOR_COMPONENT_H


class Component {
protected:
    long clock_speed = -1;
public:
    void set_clock_speed(long clock_speed);
    long get_clock_speed();

    virtual void print_stats() = 0;
};


#endif //SIMULATOR_COMPONENT_H
