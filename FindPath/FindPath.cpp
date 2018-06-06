/*
Search using A*

Create Point-> Expand point-> check if solved-> repeat

All points contain their move list, id, and esstimated cost

It is possible to easily add some additional features such as different terains, disadvantage to turning and so on.

MapSearch is created so that points don't have to contain too much information.

I'm using Manhatan instead of eucelidan because it's not allowed to move diagonally, so path length should be the same
I checked for memory leaks with visual leak detector for vs, it seems to work fine. I wasn't able to use valgrind cause I made this in visual c++ and for some reason g++ cannot read it.
In general performance is decent, but still not good enough for real time, on my laptop 100x100 map with worst case calculates 0.5s. there are some improvements if close to shortes path is ok that give 10x performance boost in some cases.
Addidng multithreading could make it couple times faster, but I haven't really considered it at the beggining, and now it's a bit difficult to fit it to this solution.

I tried using list instead of vector but performance was 30% worse
Main bottleneck is inserting elements into my expanding vector, to avoid sorting I have to insert objects in the middle of the vector
*/

#include "FindPath.h"
#include <algorithm>
#include <iostream>
using namespace std;

MapSearch::MapSearch() {
}
MapSearch::~MapSearch() {
}
MapSearch::MapSearch(const int mapWidtht, const int mapHeightt, vector<char>* mapt, vector<Point*>* toExpandt, int goalIDt)
{
	goalID = goalIDt;
	mapWidth = mapWidtht;
	mapHeight = mapHeightt;
	map = mapt;
	toExpand = toExpandt;

}
void MapSearch::Insert(Point * pnt)
{
	//toExpand->insert(toExpand->begin(),pnt);
	//Inserting element at apropriate place to avoid sorting
	// find_if looking for element with better values than the new one, and then inserting new one before that
	//this is the slowest part, but still faster than sorting
	toExpand->insert(lower_bound(toExpand->begin(), toExpand->end(), pnt, [](const Point * Apnt, const Point * Bpnt) {
		if (Apnt->EstimatedCostToGoal > Bpnt->EstimatedCostToGoal)
			return true;
		else if ((Apnt->EstimatedCostToGoal == Bpnt->EstimatedCostToGoal)) //when estimated cost is equal it prioritizes points that are closer to start to avoid blocking paths
			return (Apnt->movesTillHere > Bpnt->movesTillHere);
		else return false;
	}), pnt);

}

void MapSearch::Expand(int * id, Point * Expand)
{   //This could've been done more neatly for sure
	//I'm checking 4 things in a row
	// first: am I expanding straight back to point where I came from(removin 25% of choice)
	// 2 am I going to be out of bounds
	//3 can I go there
	//4 did I already expanded there 

	if (*id - 1 != Expand->prevPos &&
		(*id) % mapWidth != 0 &&
		(*map)[*id - 1] == 0
		)
	{
		
			(*map)[(*id - 1)] = 1;
			Point * p = new Point(Expand, Expand->movesTillHere, *id - 1);
			CalculateCost(p);
			Insert(p);
		

	}

	if (
		*id + 1 != Expand->prevPos &&
		((*id + 1) % (mapWidth) != 0 || *id == 0)&&
		(*map)[*id + 1] == 0
		)
	{
		
			(*map)[(*id + 1)] = 1;
			Point * p = new Point(Expand, Expand->movesTillHere, *id + 1);
			CalculateCost(p);
			Insert(p);
		

	}



	int k = mapWidth;
	if (*id + mapWidth != Expand->prevPos &&
		*id < (mapHeight*mapWidth - mapWidth)&&
		(*map)[*id + mapWidth] == 0
		)
	{
	
			(*map)[(*id + k)] = 1;
			Point * p = new     Point(Expand, Expand->movesTillHere, *id + mapWidth);
			CalculateCost(p);
			Insert(p);
	

	}

	if (*id - mapWidth != Expand->prevPos &&
		*id >= mapWidth&&
		(*map)[*id - mapWidth] == 0
		)
	{
		
			(*map)[(*id - k)] = 1;
			Point * p = new Point(Expand, Expand->movesTillHere, *id - mapWidth);
			CalculateCost(p);
			Insert(p);
		

	}
}

//Manhatan check
void MapSearch::CalculateCost(Point * p)
{
	// I'm extracting x and y from single ID number, that's why it looks a bit complicated
	//but basicly I'm just adding manhatan distance to amount of moves from start. 

	if (p->ID != goalID) {
		p->EstimatedCostToGoal = p->movesTillHere +
			(abs(goalID / mapWidth - p->ID / mapWidth) + abs(goalID % mapWidth - p->ID%mapWidth)); //multiply by 1.1 before parenthesis for increased performance but not guaranteed perfect solution
	}
	else {

		p->EstimatedCostToGoal = 0;
	}

	// check if not goal
}


int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize)
{
	
	
	

	// map a vector 
	vector<char>* map = new vector<char>();
	map->reserve(nMapWidth*nMapHeight);
	for (int i = 0; i < nMapWidth*nMapHeight; i++) //populating my map
	{
		if (*(pMap + i) == '\x1')
			map->push_back(0);
		else
			map->push_back(5);
	}
	if ((*map)[nTargetX + nTargetY * nMapWidth] == 5 || nTargetX>nMapWidth || nMapHeight<nTargetY) {
		return -1;

	}


	vector<Point*> * ToExpand = new vector<Point*>();
	ToExpand->reserve(nMapWidth*nMapHeight);
	vector<Point*> Garbage;
	Garbage.reserve(nMapWidth*nMapHeight);
	MapSearch * mapSearch = new MapSearch(nMapWidth, nMapHeight, map, ToExpand, nStartX + nMapWidth * nStartY);
	ToExpand->push_back(new Point(nTargetX + nTargetY * nMapWidth));

	bool solved = false;
	int CheckedStates = 0;


	//loop looks like this: expand points, sort them in order of distance to goal(witth heuristic)
	do
	{
		if (ToExpand->back()->EstimatedCostToGoal == 0)
		{
			solved = true;
			break;
		}
		if (ToExpand->back()->movesTillHere > nOutBufferSize) //when the best possible bet is out of buffer it's safe to assume that others wont make it as well                                                    
		{
			for (int k = ToExpand->size() - 1; k >= 0; k--) {
				delete (*ToExpand)[k];
			}
			for (int k = Garbage.size() - 1; k >= 0; k--) {
				delete (Garbage)[k];
			}
			Garbage.clear();
			delete mapSearch;
			delete ToExpand;

			delete map;
			
			const int bufSize2 =nOutBufferSize*2;
			//int* poutbuffer = new int[bufSize2];
			*pOutBuffer = *new int[bufSize2];
			
			return  FindPath(nStartX, nStartY, nTargetX, nTargetY, pMap, nMapWidth, nMapHeight, pOutBuffer, bufSize2);
		
		}

		//  CheckedStates++;

		Point* tmp = ToExpand->back(); // I'm using tmp because I'm going to add elements to toexpand later, andthen I wouldn't be able to pop back this element
		ToExpand->pop_back();

		mapSearch->Expand(&(tmp->ID), tmp); // expanding point, basically creating point objects in viable locations around selected point object

		(*map)[tmp->ID] = 5;
	
		Garbage.push_back(tmp);
	} while (ToExpand->size() != 0);
	//cout << "Number of expanded nodes: " << CheckedStates << endl; //display amount of checked states for diagnostics


	if (solved) {
		//fill the Buffer 
		//would be good to make temporary buffer fills to ensure that the game doesn't lag
		Point* tmp = ToExpand->back()->prev;
		for (unsigned int i = ToExpand->back()->movesTillHere; i > 0; i--)
		{
		
			*(pOutBuffer+ToExpand->back()->movesTillHere - i) = tmp->ID;
			
			tmp = tmp->prev;
			

		}

		int a = ToExpand->back()->movesTillHere;

		for (int k = ToExpand->size() - 1; k >= 0; k--) {
			delete (*ToExpand)[k];
		}
		for (int k = Garbage.size() - 1; k >= 0; k--) {
			delete (Garbage)[k];
		}
		delete mapSearch;
		delete ToExpand;
		
		delete map;
		return   a;
	}
	else
	{
		for (int k = ToExpand->size() - 1; k >= 0; k--) {
			delete (*ToExpand)[k];
		}
		for (int k = Garbage.size() - 1; k >= 0; k--) {
			delete (Garbage)[k];
		}
	
		delete mapSearch;
		delete ToExpand;
		delete map;

		return -1;
	}


}