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

    int id;                             // unique id, dont set twice
    Point2f position;                   // the actual position
    time_t bornDateTime;                // the date and time when the point was born
    vector<Touch> positionPath;         // the point history as list by Point2f

    Touch* touchWithNextDistance;        // Touch object that is next to this Touch object
    double distance;                    // the distance to the next touch object

public:

    /**
     * Sets a new touch from a absolut new finger
     */
    Touch();
    Touch(Point2f position);

    // Set shit for to do...

    void setId(int id);
    void setPosition(Point2f position);
    void setTouchWithNextDistance(Touch* touchWithNextDistance);
    void setDistance(double distance);
    void setBornNow();

    // Get shit done...

    int getId() { return this->id; }
    Point2f getPosition() { return this->position; }
    time_t getBornDate() { return this->bornDateTime; }
    vector<Touch> getPositionPath() { return this->positionPath; }
    Touch* getTouchWithNextDistance() { return this->touchWithNextDistance; }
    double getDistance() { return this->distance; }
};


#endif //MULTITOUCH_FROEMMER_TOUCH_H
