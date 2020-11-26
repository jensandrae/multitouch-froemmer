//
// Created by userx on 25.11.2020.
//

#include "Touch.h"

Touch::Touch(int position) {
    setPosition(position);
}

Touch::Touch(int id, int position) {
    setId(id);
    setPosition(position);
}

void Touch::setId(int id) {
    this->id = id;
}

void Touch::setPosition(int position) {
    this->position = position;
}

boolean Touch::isFingerNew(Point2f posOld, Point2f posNew) {
    return false;
}