#pragma once
#include "opencv2/opencv.hpp"

class TouchPoint
{
public:
	cv::Point2f point;
	int mID;

	TouchPoint(cv::Point2f p):point(p), mID(-1){}
	TouchPoint(cv::Point2f p, int id) :point(p), mID(id) {}
};

