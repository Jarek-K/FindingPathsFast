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
	EstimatedCostToGoal = 10;

}
	Point::Point(vector<int> moveList,  int cTH,int id, int  prevPost)
	{
		prevPos = prevPost;
		ID = id;
		movesTillHere = cTH;
		movesTillHere++;
		moveListTillHere = moveList;
		moveListTillHere.push_back(id);
		
		//calc expected cost
	
	}
	
	








