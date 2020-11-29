//
// Created by userx on 25.11.2020.
//
#pragma once
#include "Touch.h"

Touch::Touch() {
    this->id = int(-1);
    this->position = Point2f();
    this->bornDateTime = time(0);
    this->positionPath = vector<Touch>();
    this->touchWithNextDistance = NULL;
    this->distance = double(9999999999);


}

Touch::Touch(Point2f position) {
    this->id = int(-1);
    this->position = position;  // set
    this->bornDateTime = time(0);
    this->positionPath = vector<Touch>();
    this->touchWithNextDistance = NULL;
    this->distance = double(9999999999);
}

void Touch::setId(int id) {
    this->id = id;
}

void Touch::setPosition(Point2f position) {
    this->position = position;
}

void Touch::setTouchWithNextDistance(Touch* touchWithNextDistance) {
    this->touchWithNextDistance = touchWithNextDistance;
}

void Touch::setDistance(double distance) {
    this->distance = distance;
}

void Touch::setBornNow() {
    this->bornDateTime = std::time(0);
}