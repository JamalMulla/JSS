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

void onMouse( int event, int x, int y, int, void* );

ProcessingElement pe;

Mat Video::draw_analogue_register(AREG& reg, const string& window){
    //double minVal, maxVal;
    //minMaxLoc(reg, &minVal, &maxVal); //find minimum and maximum intensities
    //Mat draw;
    //reg.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

    cv::Mat out;
    Size size(256, 256);
    cv::resize(reg, out, size);
    setMouseCallback(window, onMouse, &reg);
    return out;
}

// Function onMouse displays cursor values
void onMouse(int event, int x, int y, int, void* reg) {
    if ( event != cv::EVENT_MOUSEMOVE )return;
    AREG* r = static_cast<AREG*>(reg);
//    x *= SCAMP_WIDTH;
//    y *= SCAMP_HEIGHT;
    std::cout<<"("<<x<<", "<<y<<") ......  "<< (int) (*r).at<int8_t>(y,x) <<'\n';
}

void Video::capture() {
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

    AREG source(SCAMP_HEIGHT, SCAMP_WIDTH, CV_8S);

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
        resize(cropFrame, cropFrame, cvSize(SCAMP_WIDTH, SCAMP_HEIGHT));
        //alpha = contrast
        //beta = brightness

        //cropFrame.copyTo(pe.PIX);
        cropFrame.convertTo(pe.PIX, MAT_TYPE, 1, -64);

        //sobel kernel
        pe.get_image(pe.A, pe.D);
//        pe.movx(pe.B, pe.A, south);
//        pe.add(pe.B, pe.B, pe.A);
//        pe.movx(pe.A, pe.B, north);
//        pe.addx(pe.B, pe.B, pe.A, east);
//        pe.sub2x(pe.A, pe.B, west, west, pe.B);

        //multiple sobel
//        pe.movx(C, A, south);
//        pe.add(B, C, A);
//        pe.addx(A, C, A, north);
//        pe.add2x(A, B, A, east, east);
//        pe.sub2x(B, A, west, west, A);
//        pe.movx(A, B, west);
//        pe.movx(B, B, west);


        // show live and wait for a key with timeout long enough to show images
        imshow("Source", draw_analogue_register(pe.PIX, "Source"));
        imshow("A", draw_analogue_register(pe.A, "A"));
        imshow("B", draw_analogue_register(pe.B, "B"));
        imshow("D", draw_analogue_register(pe.D, "D"));
        imshow("NEWS", draw_analogue_register(pe.NEWS, "NEWS"));
        //imshow("C", Video::draw_analogue_register(C));
        waitKey(1);
//        if (waitKey(5) >= 0)
//            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
}
