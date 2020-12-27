//
// Created by jm1417 on 25/12/2020.
//

#include "Video.h"
#include "PE/ProcessingElement.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <chrono>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

using namespace cv;
using namespace std;

void Video::capture() {

    int scamp_width = 256;
    int scamp_height = 256;


    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return;
    }

    //target of conversion which makes it the input register
    UMat A(scamp_height, scamp_width, CV_8S);
    UMat B(scamp_height, scamp_width, CV_8S);
    UMat C(scamp_height, scamp_width, CV_8S);

    ProcessingElement pe;

//    // start and end times
//    chrono::time_point<chrono::high_resolution_clock> start, end;
//    start = chrono::high_resolution_clock::now();
//    // fps calculated using number of frames / seconds
//    double fps;
//    // frame counter
//    int counter = 0;
//    // floating point seconds elapsed since start
//    double sec;
//    while ( true ) {
////        auto time_since_last_frame = chrono::high_resolution_clock::now() - last_frame_download;
////        std::cout << (1e3/chrono::duration_cast<chrono::milliseconds>(time_since_last_frame).count()) << "fps\r";
//
//        cap.read(frame);
//
//        end = chrono::high_resolution_clock::now();
//
//        // calculate current FPS
//        ++counter;
//        sec = chrono::duration_cast<chrono::milliseconds>(end - start).count()/1000;
//
//        fps = counter / sec;
//
//        // will print out Inf until sec is greater than 0
//        cout << fps << " fps\r" << std::flush;
////        printf("FPS = %.2f\n", fps);
//
//        imshow("Result", frame);
//        if (waitKey(1) == 27) break;
//
//    }

    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        int width = frame.cols;
        int height = frame.rows;
        Mat cropFrame = frame(Rect((width-height)/2, 0, height-1, height-1));
        resize(cropFrame, cropFrame, cvSize(scamp_width, scamp_height));
        //alpha = contrast
        //beta = brightness
        cropFrame.convertTo(A, CV_8S, 1, -128);

        //sobel kernel
         pe.movx(B, A, south);
         pe.add(B, B, A);
         pe.movx(A, B, north);
         pe.addx(B, B, A, east);
         pe.sub2x(A, B, west, west, B);

        //multiple sobel
//        pe.movx(C, A, south);
//        pe.add(B, C, A);
//        pe.addx(A, C, A, north);
//        pe.add2x(A, B, A, east, east);
//        pe.sub2x(B, A, west, west, A);
//        pe.movx(A, B, west);
//        pe.movx(B, B, west);

//        double minVal, maxVal;
//        minMaxLoc(A, &minVal, &maxVal); //find minimum and maximum intensities
//        Mat draw;
//        A.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
        // show live and wait for a key with timeout long enough to show images
        imshow("Live", A);
        waitKey(1);
//        if (waitKey(5) >= 0)
//            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
}
