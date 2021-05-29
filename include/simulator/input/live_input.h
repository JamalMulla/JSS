//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_LIVE_INPUT_H
#define SIMULATOR_LIVE_INPUT_H

#include <opencv2/videoio.hpp>

#include "input_source.h"

class LiveInput : public InputSource {
   protected:
    std::unique_ptr<cv::VideoCapture> capture;
    std::unique_ptr<cv::Size> size;

   public:
    LiveInput(int rows, int cols, int camera_index = 0);
    LiveInput() = default;

    void read(Register& reg) override;
#ifdef USE_CUDA
    cv::cuda::GpuMat& read() override;
#else
    cv::UMat& read() override;
#endif
    void reset() override;
    double last_frame_time() override;
};

#endif  // SIMULATOR_LIVE_INPUT_H
