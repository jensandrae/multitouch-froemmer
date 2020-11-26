//
// Created by userx on 25.11.2020.
//

#ifndef MULTITOUCH_FROEMMER_TOUCHCONTROLLER_H
#define MULTITOUCH_FROEMMER_TOUCHCONTROLLER_H


class TouchController {

	using namespace std;

private:

	// Stores the ID's and the points as a Point2f Object in a List as a unique pointer...
	unique_ptr<list<touch>> t0;
	shared_ptr<list<touch>> t1;
	int uniqueIdCounter;

public:
	
	TouchController();

	/**
	* Set the new List of Point2f objects
	*/
	void setNewFrame(list<Points2f> newTouches);

	/**
	* Deletes T0 and set T1 to T0
	*/
	void updateTouchLists();

	/**
	* Calculate all new Touch Objects from the given List of Point2f objects 
	* without defining the ID and store it in the t1 list
	*/
	void pointsToTouches();


	/**
	* Run the nearest neighbour algorithm on T0 and T1
	*/
	void runNNA();

	/**
	* Compare the distances between each unique point between T0 and T1 in both directions to find the shortest distance
	* And check on this way the 3 cases, new touch, move, finger up
	*/
	void processNNAresult();

	
	/**
	* Iterate thorugh all touches in T0 and inherit all ID's to the nearest neighbour
	* Set new, unique id's to all new touches
	*/
	void setNewIds();


	/**
	* Getter for the actual (processed or unprocssed) list of touches in T1
	*/
	shared_ptr<list<touch>> getActualTouches();


};


#endif //MULTITOUCH_FROEMMER_TOUCHCONTROLLER_H
