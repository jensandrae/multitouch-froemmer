//
// Created by userx on 25.11.2020.
//
#pragma once
#include "Touch.h"

Touch::Touch(Point2f position) {
    setPosition(position);
}

Touch::Touch(int id, Point2f position) {
    setId(id);
    setPosition(position);
}

void Touch::setId(int id) {
    this->id = id;
}

void Touch::setPosition(Point2f position) {
    this->position = position;
}

list<Point2f> Touch::getPositionPath()
{
    return list<Point2f>();
}
