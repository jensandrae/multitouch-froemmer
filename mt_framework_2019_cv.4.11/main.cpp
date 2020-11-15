/* Multitouch Finger Detection Framework v5.0
* for VS2019 & OpenCV 4.11
*
* Bjoern Froemmer, January 2010 - October 2019
*/

#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <Windows.h>

using namespace cv;
using namespace std;


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

    Mat frame, original, grey;

    int currentFrame = 0; // frame counter
    clock_t ms_start, ms_end, ms_time; // time

    char buffer[10]; // buffer for int to ascii conversion -> itoa(...)

    // load default image from disk
    string image_path = samples::findFile("./without_touch.jpg");
    Mat background = imread(image_path, IMREAD_GRAYSCALE);


    const char* windowName = "Fingertip detection";
    int sleep = 80;
    cv::namedWindow(windowName);
    cv::createTrackbar("sleep", windowName, &sleep, 200);

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

        //--------------------------
        // https://docs.opencv.org/4.1.1/
        // https://docs.opencv.org/2.4/modules/core/doc/basic_structures.html#vec
        // https://docs.opencv.org/2.4/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=findcontours
        //--------------------------

        // Use Point to define 2D points in an image.
        vector<vector<Point>> contours;

        // Vec4i is a structure for representing a vector with 4 dimensions, with each value an int.
        vector<Vec4i> hierarchy;

        // 
        Mat original = grey.clone();
        Mat result, blurred;

        if(background.empty())
        {
            std::cout << "Could not read the default image: " << image_path << std::endl;
            return 1;
        }

        // get the difference from original image (without touches) to touched image
        absdiff(original, background, result);
        blur(result, blurred, Size(15, 15));
        absdiff(result, blurred, result);
        blur(result, result, Size(6, 6));
        //cvtColor(result, result, COLOR_BGR2GRAY);
        // Segmentation...
        threshold(result, result, 9, 255, THRESH_BINARY);


        // After Image stuff... final.... convert image to grayscale image
        findContours(result, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

        // iterate through akk the top-level counters -> "hierarchy" may not be empty!
        if(hierarchy.size() > 0)
        {
            for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
            {
                // check contour size (number of points) and area ("blob" size)
                if(contourArea(Mat(contours.at(idx))) < 130 && contours.at(idx).size() > 4 )
                {
                    // fit & draw ellipse to counter at index
                    ellipse(original, fitEllipse(Mat(contours.at(idx))), Scalar(0,0,255), 1, 8);

                    // draw contour at index
                    drawContours(original, contours, idx, Scalar(255,0,0), 1, 8, hierarchy);
                }
            }
        }

        if (waitKey(1) == 27) // wait for user input
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

        Sleep(sleep);
    }

    std::cout << "SUCCESS: Program terminated like expected.\n";
    return 1;
}


/*


// background subtraction - use given image without touches
	absdiff(...);


// simple highpass filter
    blur(...);
	absdiff(...);
	blur(...); // optional


// threshold
	threshold(...);


//find contours
	findContours(...);


*/