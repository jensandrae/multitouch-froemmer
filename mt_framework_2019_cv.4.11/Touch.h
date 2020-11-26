//
// Created by userx on 25.11.2020.
//

#ifndef MULTITOUCH_FROEMMER_TOUCH_H
#define MULTITOUCH_FROEMMER_TOUCH_H


#include "opencv2/opencv.hpp"
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <map>
#include <Windows.h>

using namespace cv;
using namespace std;

class Touch {

private:

    int id;                          // unique id, dont set twice
    Point2f position;                // the actual position
    time_t bornDateTime;             // the date and time when the point was born
    std::list<Point2f> positionPath;// the point history as list by Point2f

public:

    /**
     * Sets a new touch from a absolut new finger
     */
    Touch(Point2f position);
    Touch(int id, Point2f position);

    void setId(int id);
    void setPosition(Point2f position);

    int getId() { return id; }
    Point2f getPosition() { return position; }
    time_t getBornDate(){ return bornDateTime; }
    list<Point2f> getPositionPath() { return positionPath; }

};


#endif //MULTITOUCH_FROEMMER_TOUCH_H
