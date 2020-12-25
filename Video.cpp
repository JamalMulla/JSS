//
// Created by jm1417 on 25/12/2020.
//

#include "Video.h"
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstdio>
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
    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
         << "Press any key to terminate" << endl;

    //target of conversion which makes it the input register
    UMat A;
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
        //cropFrame.convertTo(A, CV_32F, 1, -128);

        // show live and wait for a key with timeout long enough to show images
        imshow("Live", cropFrame);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
}
