#include "Point.h"

#include <iostream>
#include <vector>

using namespace std;


Point::Point()
{}

Point::Point(int id)
{
	prev = this;
	ID = id;
	prevPos = id;
	movesTillHere = 0;

	EstimatedCostToGoal = 10;

}
Point::Point(Point * prevt, int cTH, int id)
{
	prev = prevt;
	prevPos = prevt->ID;
	ID = id;
	movesTillHere = cTH;
	movesTillHere++;

}