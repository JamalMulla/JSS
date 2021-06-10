//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_VIDEO_INPUT_H
#define SIMULATOR_VIDEO_INPUT_H

#include "input_source.h"
#include "live_input.h"

class VideoInput : public LiveInput {
    int frame_count = 0;

   public:
    VideoInput(int rows, int cols, const std::string& path);
    void read(Register& reg) override;
#ifdef USE_CUDA
    cv::cuda::GpuMat& read() override;
#else
    cv::UMat& read() override;
#endif
};

#endif  // SIMULATOR_VIDEO_INPUT_H
