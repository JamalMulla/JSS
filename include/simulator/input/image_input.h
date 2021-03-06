//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_IMAGE_INPUT_H
#define SIMULATOR_IMAGE_INPUT_H


#include "input_source.h"

class ImageInput : public InputSource {
public:
    ImageInput(int rows, int cols, const std::string& path);
    void read(Register& reg) override;
    void reset() override;
    double last_frame_time() override;
};


#endif //SIMULATOR_IMAGE_INPUT_H
