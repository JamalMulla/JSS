//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_INPUT_SOURCE_H
#define SIMULATOR_INPUT_SOURCE_H

#include <simulator/registers/analogue_register.h>

#include <opencv4/opencv2/videoio.hpp>

enum Source { LIVE, VIDEO, IMAGE };

class InputSource {
   protected:
    cv::Mat frame;
    double time_taken = 0;
    int rows_, cols_ = 0;

   public:
    virtual void read(Register& reg) = 0;
    virtual void reset() = 0;
    virtual double last_frame_time() = 0;

    virtual ~InputSource() = default;
    ;
};

#endif  // SIMULATOR_INPUT_SOURCE_H
