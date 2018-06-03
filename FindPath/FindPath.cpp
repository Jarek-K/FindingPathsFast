/*
Search using A*

Please open in Visual Studio

Create Point-> Expand point-> check if solved-> repeat

All points contain their move list, id, and esstimated cost

It is possible to easily add some additional features such as different terains, disadvantage to turning and so on.

MapSearch is created so that point's don't have to contain too much information.

I'm using Manhatan instead of eucelidan because it's not allowed to move diagonally, so it's not technically the shortest path

I checked for memory leaks with visual leak detector for vs, it seems to work fine. I wasn't able to use valgrind cause I made this in visual c++ and for some reason g++ cannot read it.
In general performance should be good, I didn't do anything special for multithreaded environment, I think this is light enough for one thread

according to my time tracker it took my 8h to make this. Is this way to much? In my defense I didn't use c++ for a while, so I was a bit slow at the start
*/


#include <chrono>
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
	//toExpand->push_back(pnt);

	toExpand->insert(find_if(toExpand->begin(), toExpand->end(), [pnt](const Point * Tpnt) {
		if (Tpnt->EstimatedCostToGoal < pnt->EstimatedCostToGoal)
			return true;
		else if ((Tpnt->EstimatedCostToGoal == pnt->EstimatedCostToGoal))
			return (Tpnt->movesTillHere < pnt->movesTillHere);
		else return false;
	}),pnt);


	//for (vector<Point*>::iterator it = toExpand->begin(); it < toExpand->end(); i++)
	//{
	//	*it
	////	if(*it.>2)

	//	/*	if (a->EstimatedCostToGoal != b->EstimatedCostToGoal)
	//			return (
	//				a->EstimatedCostToGoal > b->EstimatedCostToGoal
	//				);
	//	return(a->movesTillHere > b->movesTillHere);*/
	//}
}

void MapSearch::Expand(int * id, Point * Expand, vector<int>* Expanded)
{	//This could've been done more neatly for sure
	 //I'm checking 4 things in a row
	 // first: am I expanding straight back to point where I came from(removin 25% of choice)
	 // 2 am I going to be out of bounds
	 //3 can I go there
	 //4 did I already expanded there 
	if (*id - 1 != Expand->prevPos &&
		(*id) % mapWidth != 0 &&
		(*map)[*id - 1] == '\x1'
		&& (*Expanded)[(*id - 1)] == 0

		)
	{
		(*Expanded)[(*id - 1)] = 2;
		Point * p = new Point(Expand->moveListTillHere, Expand->movesTillHere, *id - 1);
		CalculateCost(p);
		Insert(p);
		
	}

	if (
		*id + 1 != Expand->prevPos &&
		((*id + 1) % (mapWidth) != 0 || *id == 0) &&
		(*map)[*id + 1] == '\x1'
		&& (*Expanded)[*id + 1] == 0

		)
	{
		(*Expanded)[(*id + 1)] = 2;
		Point * p = new Point(Expand->moveListTillHere, Expand->movesTillHere, *id + 1);
		CalculateCost(p);
		Insert(p);
	
	}
	int k = mapWidth;
	if (*id + mapWidth != Expand->prevPos &&
		*id < (mapHeight*mapWidth - mapWidth) &&
		(*map)[*id + mapWidth] == '\x1'
		&& (*Expanded)[*id + k] == 0

		)
	{
		(*Expanded)[(*id+ k)] = 2;
		Point * p = new 	Point(Expand->moveListTillHere, Expand->movesTillHere, *id + mapWidth);
		CalculateCost(p);
		Insert(p);
		
	}

	if (*id - mapWidth != Expand->prevPos &&
		*id >= mapWidth &&
		(*map)[*id - mapWidth] == '\x1'
		&& (*Expanded)[*id - k] == 0

		)
	{
		(*Expanded)[(*id -k)] = 2;
		Point * p = new	Point(Expand->moveListTillHere, Expand->movesTillHere, *id - mapWidth);
		CalculateCost(p);
		Insert(p);

	}

}

//Manhatan check
void MapSearch::CalculateCost(Point * p)
{
	// I'm extracting x and y from single ID number, that's why it looks a bit complicated
	//but basicly I'm just adding manhatan distance to amount of moves from start, so a*. I should also add some prefrence for distance 
	//but I wanted estimated distance to be an int, and alsow I want perfect solution every time
	//later this quantity is used to sort elements to be expanded
	if (p->ID != goalID) {
		p->EstimatedCostToGoal = p->movesTillHere +
			(abs(p->ID / mapWidth - goalID / mapWidth) + abs(p->ID%mapWidth - goalID % mapWidth)); //add 1.1 before parenthesis for increased performance but not guaranteed perfect solution
		
		
	}
	else {
		cout << "end";
		p->EstimatedCostToGoal = 0;

	}

	// check if not goal
}

int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize)
{
	//make map real 
	vector<char>* map = new vector<char>();
	map->reserve(nMapWidth*nMapHeight);
	for (int i = 0; i < nMapWidth*nMapHeight; i++) //populating my map
	{
		map->push_back(*(pMap + i));
	}
	vector <int> Expanded(nMapWidth*nMapHeight);
	//Expanded.reserve(nMapWidth*nMapHeight);
	
	vector<Point*> * ToExpand = new vector<Point*>();
	ToExpand->reserve(0.1*nMapWidth*nMapHeight);
	MapSearch * mapSearch = new MapSearch(nMapWidth, nMapHeight, map, ToExpand, nTargetX + nTargetY * nMapWidth);
	ToExpand->push_back(new Point(nStartX + nMapWidth * nStartY));

	bool solved = false;
	int CheckedStates = 0;

	auto start = chrono::high_resolution_clock::now();
	//loop looks like this: expand points, sort them in order of distance to goal(witth heuristic)
	chrono::duration<float> duration2;
	float Time = 0;
	do
	{
		
		if ((*ToExpand)[ToExpand->size()-1]->movesTillHere >= nOutBufferSize) //when the best possible bet is out of buffer it's safe to assume that others wont make it as well
															
		{
			//I've decided that returning -1 makes more sense than calling another function with higher buffer, cause buffer is specified for some reason and it also might be a gamplay feature. 
			break;
		}

		CheckedStates++;
		Point* tmp = ToExpand->back(); // I'm using tmp because I'm going to add elements to toexpand later, andthen I wouldn't be able to pop back this element
		ToExpand->pop_back();
		auto start2 = chrono::high_resolution_clock::now();
		mapSearch->Expand(&(tmp->ID), tmp, &Expanded); // expanding point, basically creating point objects in viable locations around selected point object
		auto end2 = chrono::high_resolution_clock::now();
		Expanded[tmp->ID] = 1;
		//Expanded.push_back(tmp->ID);
		delete tmp;
		
	
			if (ToExpand->back()->EstimatedCostToGoal == 0) 
			{
				solved = true;
				break;
			}

				//sort(ToExpand->begin(), ToExpand->end(),
				//	[](const Point* a, const Point* b) //I could hide it but let's be fair I got idea of using lambdas in sort from stack overflow 
				//									   // I also edited it to use pointers to my class and whatnot
				//{										//and I added the 2nd expresion so that when estimated cost is equal, lower moves value has a prefrence
				//	if (a->EstimatedCostToGoal != b->EstimatedCostToGoal)
				//		return (
				//			a->EstimatedCostToGoal > b->EstimatedCostToGoal
				//			);
				//	return(a->movesTillHere > b->movesTillHere);
				//});
				
			 duration2 =  (end2 - start2);
			 Time += duration2.count();



	} while (ToExpand->size() != 0);
	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;
	cout << "Time to find Path clean: " << duration.count() << endl; //baseline 0.007-8
	cout << "Time to sort: " << Time << endl; //baseline 0.007-8
	cout << CheckedStates << endl; //display amount of checked states for diagnostics
	//fill the Buffer 
	//would be good to make temporary buffer fills to ensure that the game doesn't lag

	Expanded.clear();
	if (solved) {
		for (unsigned int i = 0; i < ToExpand->back()->moveListTillHere.size(); i++)
		{
			pOutBuffer[i] = ToExpand->back()->moveListTillHere[i];
			//cout << pOutBuffer[i] << " ";  //display buffer to check if everything is ok
		}

		int a = ToExpand->back()->movesTillHere;

		for (int k = ToExpand->size() - 1; k > 0; k--) {
			delete (*ToExpand)[k];
		}
		delete mapSearch;
		delete ToExpand;
		delete map;
		return   a;
	}
	else
	{
		for (int k = ToExpand->size(); k > 0; k--) {
			delete (*ToExpand)[k - 1];
		}

		delete mapSearch;
		delete ToExpand;
		delete map;
		return -1;
	}


}


