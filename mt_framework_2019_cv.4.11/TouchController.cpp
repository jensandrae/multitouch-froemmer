//
// Created by userx on 25.11.2020.
//

#include "TouchController.h"

TouchController::TouchController() {
    this->t0 = make_shared<list<Touch>>();
    this->t1 = make_shared<list<Touch>>();
    uniqueIdCounter = 0;
}


void TouchController::setNewFrame(list<Point2f> newTouches) {
    updateTouchLists();
    t1 = std::make_shared<list<touch>>(newTouches);
}


void TouchController::updateTouchLists() {
    t0 = std::make_shared<list<touch>>(t1);
}


void TouchController::pointsToTouches() {

}

void TouchController::runNNA() {

}

void TouchController::processNNAresult() {

}


void TouchController::setNewIds() {

}

shared_ptr<list<touch>> TouchController::getActualTouches() {
    return null;
}
