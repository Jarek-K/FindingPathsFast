#include "Point.h"

#include <iostream>
#include <vector>
#include <map> //I'm not sure if solution with map wouldn't be faster
#include <algorithm>
using namespace std;


Point::Point()
{}
Point::Point(int id)
{
	
	ID = id;
	prevPos = id;
	movesTillHere = 0;
	moveListTillHere = vector<int>();

}
	Point::Point(vector<int> moveList,  int cTH,int id, int  prevPost)
	{
		prevPos = prevPost;
		ID = id;
		movesTillHere = cTH;
		movesTillHere++;
		moveListTillHere = moveList;
		moveListTillHere.push_back(id);
		EstimatedDistanceToGoal = 10;// initializing it with 10 because don't want the program think that it's insta solve
		//calc expected cost
	
	}
	
	








