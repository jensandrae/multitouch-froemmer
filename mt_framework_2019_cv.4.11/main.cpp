/* Multitouch Finger Detection Framework v5.0
* for VS2019 & OpenCV 4.11
*
* Bjoern Froemmer, January 2010 - October 2019
*/
#pragma once

#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <Windows.h>
#include "TouchPoint.h"

//
#define TOUCH_MAX_CUTOFF_DIST 8000
int touchPointID = 0;

using namespace cv;
using namespace std;

double squareDistance(TouchPoint first, TouchPoint second) {
    double x = abs(first.point.x - second.point.x);
    double y = abs(first.point.y - second.point.y);
    double squareDist = x * x + y * y;
    return squareDist;
}

std::vector<TouchPoint> nearestNeighbor(std::vector<cv::Point2f> currentPoints, std::vector<TouchPoint> previousPoints)
{
    vector<TouchPoint> newPoints;
    //Dirty Hack!!!
    double closestDist = 9999999;
    //The index of the closest Element in the previous Frame
    int closestTouchPointIdxT0 = -1;
    bool foundPoint = false;

    for (int i = 0; i < currentPoints.size();) {
        for (int j = 0; j < previousPoints.size(); j++) {

            //Calc dist
            double dist = squareDistance(currentPoints.at(i), previousPoints.at(j));
            if (dist < closestDist && dist < TOUCH_MAX_CUTOFF_DIST) {
                closestDist = dist;
                closestTouchPointIdxT0 = j;
                foundPoint = true;
            }
            // Only for Debug
            //std::cout << dist << "\n";
        }
        //std::cout << closestDist << "\n";
        TouchPoint tp(currentPoints.at(i),(foundPoint) ? previousPoints.at(closestTouchPointIdxT0).mID : ++touchPointID);
        if (foundPoint) {
            previousPoints.erase(previousPoints.begin() + closestTouchPointIdxT0);   
        }
        currentPoints.erase(currentPoints.begin());
        newPoints.push_back(tp);
        
        closestTouchPointIdxT0 = -1;
        foundPoint = false;
        //Dirty Hack!!!
        closestDist = 9999999;
    }
    return newPoints;
}

int main(void)
{
    //VideoCapture cap(0); // use the first camera found on the system
    VideoCapture cap("../mt_camera_raw.AVI");

    if (!cap.isOpened())
    {
        std::cout << "ERROR: Could not open camera / video stream.\n";
        return -1;
    }

    // get the frame size of the videocapture object
    double videoWidth = cap.get(CAP_PROP_FRAME_WIDTH);
    double videoHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

    Mat frame, original, grey, refImg, onlyContours;

    int currentFrame = 0; // frame counter
    clock_t ms_start, ms_end, ms_time; // time

    char buffer[10]; // buffer for int to ascii conversion -> itoa(...)

    RotatedRect actualEllipse;

    const char* windowName = "Fingertip detection";
    int wKey = 80;
    //Old values: 10, 4, 30, 4
    int kernelSize = 10, minV = 3, maxV = 26, thresh = 4;
    cv::namedWindow(windowName);
    cv::createTrackbar("WaitKey", windowName, &wKey, 200);
    cv::createTrackbar("High-Pass-Filter", windowName, &kernelSize, 100);
    cv::createTrackbar("ContourArea", windowName, &maxV, 100);
    cv::createTrackbar("Contour Size", windowName, &minV, 30);
    cv::createTrackbar("Threshold", windowName, &thresh, 100);
    bool firstFrame = true;

    vector<TouchPoint> previousPoints;
    vector<Point2f> currentPoints;    
    
    for (;;)
    {
        ms_start = clock(); // time start

        cap >> frame; // get a new frame from the videostream

        if (frame.data == NULL) // terminate the program if there are no more frames
        {
            std::cout << "TERMINATION: Camerastream stopped or last frame of video reached.\n";
            break;
        }

        // copy frame to original
        original = frame.clone();

        // convert frame to greyscale image (copies the image in the process!)
        cvtColor(original, grey, COLOR_BGR2GRAY);

        //Calibrate with first frame
        if (firstFrame) {
            firstFrame = false;
            cvtColor(original, refImg, COLOR_BGR2GRAY);
        }

        //--------------------------
        // https://docs.opencv.org/4.1.1/
        // https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#vec
        // https://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours
        //--------------------------

        // Use Point to define 2D points in an image.
        vector<vector<Point>> contours;

        // Vec4i is a structure for representing a vector with 4 dimensions, with each value an int.
        vector<Vec4i> hierarchy;

        Mat original = grey.clone();
        Mat result, blurred;

        // get the difference from original image (without touches) to touched image
        absdiff(refImg, grey, result);
        // High-Pass-Filter
        blur(result, blurred, Size(kernelSize, kernelSize));
        absdiff(result, blurred, result);
        blur(result, result, Size(kernelSize, kernelSize));
        
        threshold(result, result, thresh, 255, THRESH_BINARY);

        // After Image stuff... final.... convert image to grayscale image
        findContours(result, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        // P2 - Clear the currentPoints list for each new frame...
        currentPoints.clear();

        // iterate through akk the top-level counters -> "hierarchy" may not be empty!
        if(hierarchy.size() > 0)
        {
            for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
            {
                // check contour size (number of points) and area ("blob" size)
                double conArea= contourArea(Mat(contours.at(idx)));
                if(conArea > maxV && contours.at(idx).size() > minV)
                {

                    // P2 - get the center of a ellipse that was generated based on the finger blob
                    actualEllipse = fitEllipse(Mat(contours.at(idx)));

                    // fit & draw ellipse to counter at index
                    ellipse(original, actualEllipse, Scalar(0,0,255), 1, 8);

                    // draw contour at index
                    drawContours(original, contours, idx, Scalar(255,0,0), 1, 8, hierarchy);

                    // P2 - now we can get the center information from the finger blob based ellipse
                   
                    currentPoints.push_back(actualEllipse.center);             
                }
            }
        }
        
        std::vector<TouchPoint> newPoints = nearestNeighbor(currentPoints, previousPoints);
        // P2 - After all contours, ellipses and ellipse.center as Point2f were processed...
        // Reassign touchPoints 
        previousPoints = newPoints;
        
        for (TouchPoint &vTouch : previousPoints) {
            //Draw IDs for each finger
            putText(original, to_string(vTouch.mID), vTouch.point, FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1);
        } 

        if (waitKey(wKey) == 27) // wait for user input
        {
            std::cout << "TERMINATION: User pressed ESC\n";
            break;
        }

        currentFrame++;

        // time end
        ms_end = clock();
        ms_time = ms_end - ms_start;

        // write framecounter to the image (useful for debugging)
        putText(original, "frame #" + (std::string)_itoa(currentFrame, buffer, 10), Point(0, 15), FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); 
        putText(grey, "frame #" + (std::string)_itoa(currentFrame, buffer, 10), Point(0, 15), FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);

        // write calculation time per frame to the image
        putText(original, "time per frame: " + (std::string)_itoa(ms_time, buffer, 10) + "ms", Point(0, 30), FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);
        putText(grey, "time per frame: " + (std::string)_itoa(ms_time, buffer, 10) + "ms", Point(0, 30), FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8);

        // render the frame to a window
        imshow("Stream - Original", original); 
        imshow("Stream - Processed", result);

    }

    std::cout << "SUCCESS: Program terminated like expected.\n";
    return 1;
}