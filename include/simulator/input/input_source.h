//
// Created by jm1417 on 06/03/2021.
//

#ifndef SIMULATOR_INPUT_SOURCE_H
#define SIMULATOR_INPUT_SOURCE_H

#include <simulator/registers/analogue_register.h>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

enum Source { LIVE, VIDEO, IMAGE };

class InputSource {
   protected:
#ifdef USE_CUDA
    cv::cuda::GpuMat frame;
#else
    cv::UMat frame;
#endif
    double time_taken = 0;
    int rows_, cols_ = 0;

   public:
    virtual void read(Register& reg) = 0;
#ifdef USE_CUDA
    virtual cv::cuda::GpuMat& read() = 0;
#else
    virtual cv::UMat& read() = 0;
#endif

    virtual void reset() = 0;
    virtual double last_frame_time() = 0;

    virtual ~InputSource() = default;
    ;
};

#endif  // SIMULATOR_INPUT_SOURCE_H
