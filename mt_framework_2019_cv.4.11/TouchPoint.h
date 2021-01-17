#pragma once
#include "opencv2/opencv.hpp"
#include "../TUIO/TuioCursor.h"
#include <memory>

class TouchPoint
{
public:
	std::shared_ptr<TUIO::TuioCursor> tcur;
	cv::Point2f point;
	int mID;

	TouchPoint(){};
	TouchPoint(cv::Point2f p):point(p), mID(-1), tcur(nullptr){}
	TouchPoint(cv::Point2f p, int id) :point(p), mID(id), tcur(nullptr) {}
	TouchPoint(cv::Point2f p, int id, std::shared_ptr<TUIO::TuioCursor> cursor) :point(p), mID(id), tcur(cursor) {}

	~TouchPoint() {}
};

