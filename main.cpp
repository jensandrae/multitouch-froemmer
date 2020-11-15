/* Multitouch Finger Detection Framework v5.0
* for VS2019 & OpenCV 4.11
*
* Bjoern Froemmer, January 2010 - October 2019
*/

#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#include <math.h>

int main(void)
{
	//VideoCapture cap(0); // use the first camera found on the system
	cv::VideoCapture cap("../mt_camera_raw.AVI");

	if (!cap.isOpened())
	{
		std::cout << "ERROR: Could not open camera / video stream.\n";
		return -1;
	}

	// get the frame size of the videocapture object
	double videoWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	double videoHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

	cv::Mat frame, original, grey;

	int currentFrame = 0; // frame counter
	clock_t ms_start, ms_end, ms_time; // time

	char buffer[10]; // buffer for int to ascii conversion -> itoa(...)

	for (;;)
	{
		ms_start = clock(); // time start

		cap >> frame; // get a new frame from the videostream

		if (frame.data == NULL) // terminate the program if there are no more frames
		{
			std::cout << "TERMINATION: Camerastream stopped or last frame of video reached.\n";
			break;
		}

		original = frame.clone(); // copy frame to original

		cvtColor(original, grey, cv::COLOR_BGR2GRAY); // convert frame to greyscale image (copies the image in the process!)

		//--------------------------
		// your code goes here
		//--------------------------

		if (cv::waitKey(1) == 27) // wait for user input
		{
			std::cout << "TERMINATION: User pressed ESC\n";
			break;
		}

		currentFrame++;

		// time end
		ms_end = clock();
		ms_time = ms_end - ms_start;

		putText(original, "frame #" + (std::string)_itoa(currentFrame, buffer, 10), cv::Point(0, 15), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write framecounter to the image (useful for debugging)
		putText(original, "time per frame: " + (std::string)_itoa(ms_time, buffer, 10) + "ms", cv::Point(0, 30), cv::FONT_HERSHEY_PLAIN, 1, CV_RGB(255, 255, 255), 1, 8); // write calculation time per frame to the image

		imshow("window", original); // render the frame to a window	
	}

	std::cout << "SUCCESS: Program terminated like expected.\n";
	return 1;
}