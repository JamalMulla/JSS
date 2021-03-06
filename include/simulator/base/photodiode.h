//
// Created by jm1417 on 28/01/2021.
//

#ifndef SIMULATOR_PHOTODIODE_H
#define SIMULATOR_PHOTODIODE_H


#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <simulator/input/input_source.h>
#include "simulator/registers/analogue_register.h"
#include "simulator/base/component.h"

class Photodiode : public Component {
private:
    std::shared_ptr<InputSource> input_source;
public:
    Photodiode(int rows, int cols, Source src, const std::string& path);
    void reset();
    void read(Register& reg);
    double last_frame_time();
    void print_stats(const CycleCounter& counter) override;
    void write_stats(const CycleCounter& counter, json& j) override;
    ~Photodiode();
};


#endif //SIMULATOR_PHOTODIODE_H
