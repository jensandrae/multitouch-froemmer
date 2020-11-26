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
	unique_ptr<list<touch>> t1;

	Mat acutalImage;

public:
	
	TouchController();
	TouchController(list<Touch*> touches, Mat original);

	// 1. Set the new image for t1 as a Mat object
	//    Sonderfälle: Erster Bild, Nicht erstes Bild

	// 2. Set t1 to t0 so that we can build for t1 the new set of points and ids
	
	// 3. Calculate all new points and ids on t1

	// 4. Nearesed Neighbour Algorithm 

	// Finger Neu
	// Finger Alt
	// Finger hoch gehoben







};


#endif //MULTITOUCH_FROEMMER_TOUCHCONTROLLER_H
