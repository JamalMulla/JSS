//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_LIVE_INPUT_H
#define SIMULATOR_LIVE_INPUT_H

#include <opencv4/opencv2/videoio.hpp>

#include "input_source.h"

class LiveInput : public InputSource {
   protected:
    std::unique_ptr<cv::VideoCapture> capture;
    std::unique_ptr<cv::Size> size;

   public:
    LiveInput(int rows, int cols);
    LiveInput();

    void read(Register& reg) override;
    cv::Mat read() override;
    void reset() override;
    double last_frame_time() override;
};

#endif  // SIMULATOR_LIVE_INPUT_H
