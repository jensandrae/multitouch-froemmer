//
// Created by userx on 25.11.2020.
//
#pragma once
#include "TouchController.h"

TouchController::TouchController() {
    this->t0 = new vector<Touch*>();
    this->t1 = new vector<Touch*>();
    this->newTouches = vector<Point2f>();
    this->uniqueIdCounter = 1;
}

/**
* Set the new List of Point2f objects
*/
vector<Touch*> TouchController::calcNewFrame(vector<Point2f> newTouches) {

    this->newTouches.clear();
    this->newTouches = newTouches;

    updateTouchLists();
    pointsToTouches();
    runNNA();
    processNNAresult();


    // ToDo: return something to main....
    return getActualTouches();
}

/**
* Deletes T0 and set T1 to T0
*/
void TouchController::updateTouchLists() {
    t0 = new vector<Touch*>();
    *t0 = *t1;
    t1 = new vector<Touch*>();
}

/**
* Calculate all new Touch Objects from the given List of Point2f objects
* without defining the ID and store it in the t1 list
*/
void TouchController::pointsToTouches() {

    for (Point2f p : newTouches) {
        this->t1->push_back(new Touch(p));
    }
}

/**
* Run the nearest neighbour algorithm on T0 and T1
*/
void TouchController::runNNA() {

    // reset the neighbour information, so that the points without neighbour will have a nullptr
    vector<Touch*>::iterator iterT0;
    for (iterT0 = this->t0->begin(); iterT0 != this->t0->end(); ++iterT0) {
        (*iterT0)->setTouchWithNextDistance(NULL);
    }

    // No touches in the last frame (t0)...
    if (this->t0->size() == 0) {

        // ...and no touches in new frame (t1)
        if (this->t1->size() == 0) {
            // Nothing to do here, no neighbours no work...
        }

        // ...and new touches in new frame (t1), so all touches in the new frame (t1) are new fingers
        // no finger moves, no finger ups, all finger new
        if (this->t1->size() > 0) {
            // we can set all neightbours from (t1) to null, what means no neighbour there
            // See constructor "Touch.cpp" everything gets there done...
        }
    }

        // There are touches in the last frame (t0)...
    else if (this->t0->size() > 0) {

        // ...but in the new frame (t1) are no touches any more, so all finger moved up
        if (this->t1->size() == 0) {
            // Nothing to do here, all fingers are gone...
        }

        // ...and the new frame (t1) are also touches, now decide between [finger new, finger move, finger up]
        if (this->t1->size() > 0) {
            // LOOP: Find all NN's and set it to the Touch Objects....

            vector<Touch*>::iterator iterT0;
            for (iterT0 = this->t0->begin(); iterT0 != this->t0->end(); ++iterT0) {

                Touch* t1_old = NULL;

                vector<Touch*>::iterator iterT1;
                for (iterT1 = this->t1->begin(); iterT1 != this->t1->end(); ++iterT1) {

                    if (t1_old == NULL)
                        t1_old = *iterT1;

                    // Calculate the distance between the two gieven point...
                    // ...from one t0 point to all given t1 points
                    double actual_distance = calcDistance((*iterT0)->getPosition(), (*iterT1)->getPosition());

                    // Check if the actual distance is greater than the old
                    // If not, set the actual distance to the new distance and process....

                    if ( actual_distance < t1_old->getDistance() ) {
                        (*iterT0)->setTouchWithNextDistance(*iterT1);
                        (*iterT0)->setDistance(actual_distance);
                        (*iterT1)->setTouchWithNextDistance(*iterT0);
                        (*iterT1)->setDistance(actual_distance);
                    }

                    // After all, set last distance to the actual distance
                }
            }
        }
    }

    vector<Touch*>::iterator iterT1;
    for (iterT1 = this->t1->begin(); iterT1 != this->t1->end(); ++iterT1) {

        Touch* t0_old = NULL;

        vector<Touch*>::iterator iterT0;
        for (iterT0 = this->t0->begin(); iterT0 != this->t0->end(); ++iterT0) {

            if (t0_old == NULL)
                t0_old = *iterT1;

            // Calculate the distance between the two gieven point...
            // ...from one t0 point to all given t1 points
            double actual_distance = calcDistance((*iterT0)->getPosition(), (*iterT1)->getPosition());

            // Check if the actual distance is greater than the old
            // If not, set the actual distance to the new distance and process....

            if (actual_distance < t0_old->getDistance()) {
                (*iterT0)->setTouchWithNextDistance(*iterT1);
                (*iterT0)->setDistance(actual_distance);
                (*iterT1)->setTouchWithNextDistance(*iterT0);
                (*iterT1)->setDistance(actual_distance);
            }

            // After all, set last distance to the actual distance
        }
    }

}

/**
* Compare the distances between each unique point between T0 and T1 in both directions to find the shortest distance
* And check on this way the 3 cases, new touch, move, finger up
*/
void TouchController::processNNAresult() {

    cout << "::: processNNAresult() ... " << endl;

    vector<Touch*>::iterator iterT0;
    cout << "--------- iterT0 ---------" << endl;
    for (iterT0 = this->t0->begin(); iterT0 != this->t0->end(); ++iterT0) {
        cout << " id " << (*iterT0)->getId() << " -  distance " << (*iterT0)->getDistance() << endl;
    }

    cout << "--------- iterT1 ---------" << endl;
    vector<Touch*>::iterator iterT1;
    for (iterT1 = this->t1->begin(); iterT1 != this->t1->end(); ++iterT1) {
        cout << " id " << (*iterT1)->getId() << " -  distance " << (*iterT1)->getDistance() << endl;
    }


    // No touches in the last frame (t0)...
    if (this->t0->size() == 0) {

        // ...and no touches in new frame (t1)
        if (this->t1->size() == 0) {
            // Nothing to do here...
            cout << "this->t1->size() == 0" << endl;
        }

        // ...and new touches in new frame (t1), so all touches in the new frame (t1) are new fingers
        // no finger moves, no finger ups
        if (this->t1->size() > 0) {
            cout << "this->t1->size() > 0" << endl;
            vector<Touch*>::iterator iterT1;
            for (iterT1 = this->t1->begin(); iterT1 != this->t1->end(); ++iterT1) {
                (*iterT1)->setId(getNextFreeId());
            }
        }
    }

        // There are touches in the last frame (t0)...
        // Be aware, there can only be one element in list, a single element without a neighbour
    else if (this->t0->size() > 0) {

        // ...but in the new frame (t1) are no touches any more, so all finger moved up
        if (this->t1->size() == 0) {
            // Nothing to do here....
        }

        // ...and the new frame (t1) are also touches, now decide between [finger new, finger move, finger up]
        if (this->t1->size() > 0) {
            vector<Touch*>::iterator iterT1;
            for (iterT1 = this->t1->begin(); iterT1 != this->t1->end(); ++iterT1) {

                // How you can see if a touch is new, moved or not present anymore ?
                // new -> new id
                // moved -> old id
                // up -> not present, no id to show

                // FINGER UP ::: Finger ups are at this point not present anymore, because they are not in t1
                // So they wont get retured after that function return to main...


                // NEW FINGER ::: Is not a move, so it can only be a new finger
                if ((*iterT1)->getDistance() > MAX_NEIGHBOUR_DISTANCE) {
                    // tElement1.setTouchWithNextDistance(nullptr);
                    (*iterT1)->setId(getNextFreeId());
                }

                // MOVE ::: Is a move...
                if ((*iterT1)->getDistance() <= MAX_NEIGHBOUR_DISTANCE) {
                    // ToDo: Add Element to history to specific Touch
                    // ToDo: WHY HERE ERROR
                    (*iterT1)->setId((*iterT1)->getTouchWithNextDistance()->getId());
                }
                // tElement1.setId(-99);
            }
        }
    }


    // At this point new finger are new cause the get a new unique id, finger ups are not present cause this are points
}

/**
* Iterate thorugh all touches in T0 and inherit all ID's to the nearest neighbour
* Set new, unique id's to all new touches
*/
int TouchController::getNextFreeId() {
    return this->uniqueIdCounter++;
}

/**
* Getter for the actual (processed or unprocssed) list of touches in T1
*/
vector<Touch*> TouchController::getActualTouches() {
    return *t1;
}

/**
* Calculate the distance between the two given points
*/
double TouchController::calcDistance(Point2f first, Point2f second) {
    double x = abs(first.x - second.x);
    double y = abs(first.y - second.y);
    return sqrt(x * y * y);
}