



#include "FinderHeader.h"
#include <algorithm>
#include <iostream>
using namespace std;
MapSearch::MapSearch() {
}
MapSearch::~MapSearch() {
}
MapSearch::MapSearch(const int mapWidtht, const int mapHeightt, vector<int>* mapt, vector<Point*>* toExpandt, int goalIDt)
{
	goalID = goalIDt;
	mapWidth = mapWidtht;
	mapHeight = mapHeightt;
	map = mapt;
	toExpand = toExpandt;

}
void MapSearch::Expand(int * id, Point * Expand, vector<int>* Expanded)
{	//This could've been done more neatly for sure
	 //I'm checking for things in a row
	 // first: am I expanding straight back to point where I came from(removin 25% of choice)
	 // 2 am I going to be out of bounds
	 //3 can I go there
	 //4 did I already expanded there (1 only check last move)
	if (*id - 1 != Expand->prevPos &&
		(*id) % mapWidth != 0 &&
		(*map)[*id - 1] == 1
		&& find_if(toExpand->begin(), toExpand->end(), [id](const Point * Pnt) {return Pnt->ID == *id - 1; }) == toExpand->end()
		&& find(Expanded->begin(),Expanded->end(), *id - 1)==Expanded->end()
		)
	{

		Point * p = new Point(Expand->moveListTillHere, Expand->movesTillHere, *id - 1, *id);
		CalculateCost(p);
		toExpand->push_back(p);
	}

	if (
		*id + 1 != Expand->prevPos &&
		((*id + 1) % (mapWidth) != 0 || *id == 0) &&
		(*map)[*id + 1] == 1
		&& find_if(toExpand->begin(), toExpand->end(), [id](const Point * Pnt) {return Pnt->ID == *id + 1; }) == toExpand->end()
		&& find(Expanded->begin(), Expanded->end(), *id + 1) == Expanded->end()
		)
	{
		Point * p = new Point(Expand->moveListTillHere, Expand->movesTillHere, *id + 1, *id);
		CalculateCost(p);
		toExpand->push_back(p);
	}
	int k = mapWidth;
	if (*id + mapWidth != Expand->prevPos &&
		*id < (mapHeight*mapWidth - mapWidth) &&
		(*map)[*id + mapWidth] == 1
		&& find_if(toExpand->begin(), toExpand->end(), [id, k](const Point * Pnt) {return Pnt->ID == *id + k; }) == toExpand->end()
		&& find(Expanded->begin(), Expanded->end(), *id +k) == Expanded->end()
		)
	{
		Point * p = new 	Point(Expand->moveListTillHere, Expand->movesTillHere, *id + mapWidth, *id);
		CalculateCost(p);
		toExpand->push_back(p);
	}

	if (*id - mapWidth != Expand->prevPos &&
		*id >= mapWidth &&
		(*map)[*id - mapWidth] == 1
		&& find_if(toExpand->begin(), toExpand->end(), [id, k](const Point * Pnt) {return Pnt->ID == *id - k; }) == toExpand->end()
		&& find(Expanded->begin(), Expanded->end(), *id +k) == Expanded->end()
		)
	{
		Point * p = new 	Point(Expand->moveListTillHere, Expand->movesTillHere, *id - mapWidth, *id);
		CalculateCost(p);
		toExpand->push_back(p);
	}

}

//Manhatan check
void MapSearch::CalculateCost(Point * p)
{
	// I'm extracting x and y from single ID number, that's why it looks a bit complicated
	//but basicly I'm just adding manhatan distance to amount of moves from start, so a*. I should also add some prefrence for distance 
	//but I wanted estimated distance to be an int, and alsow I want perfect solution every time
	//later this quantity is used to sort elements to be expanded
	if (p->ID != goalID)
		p->EstimatedCostToGoal = p->movesTillHere +
		(abs((p->ID - ((p->ID) / mapWidth)*mapWidth) - (goalID - (goalID / mapWidth)*mapWidth)) +  //add 1.1 before parenthesis for increased performance but not guaranteed perfect solution
			abs((p->ID / mapWidth) - (goalID / mapWidth)));
	else {
		p->EstimatedCostToGoal = 0;
		delete (*toExpand)[0];
		(*toExpand)[0] = p;
	}

	// check if not goal
}

int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize)
{
	//make map real 
	vector<int>* map = new vector<int>(); // Probably should've stayed with chars for map, but  for some reason int's look nicer to me, but than again memory is waisted
	for (int i = 0; i < nMapWidth*nMapHeight; i++) //populating my map
	{
		unsigned char p = '\x1';

		if (*(pMap + i) == p)
			map->push_back(1);
		else
		{
			map->push_back(0);
		}

	}
	vector <int> Expanded;
	vector<Point*> * ToExpand = new vector<Point*>();
	MapSearch * mapSearch = new MapSearch(nMapWidth, nMapHeight, map, ToExpand, nTargetX + nTargetY * nMapWidth);
	ToExpand->push_back(new Point(nStartX + nMapWidth * nStartY));

	bool solved = false;
	int CheckedStates = 0;


	//loop looks like this: expand points, sort them in order of distance to goal(witth heuristic)
	do
	{

		CheckedStates++;
		Point* tmp = ToExpand->back(); // I'm using tmp because I'm going to add elements to toexpand later, andthen I wouldn't be able to pop back this element
		ToExpand->pop_back();//remember to ad destruction
		mapSearch->Expand(&(tmp->ID), tmp, &Expanded); // expanding point, basically creating point objects in viable locations around selected point object
		Expanded.push_back(tmp->ID);
		delete tmp;
		if (ToExpand->size() != 0) {
			if ((*ToExpand)[0]->EstimatedCostToGoal == 0) // I'm checking at 0 because during expansion when solution is detected it's pushed to 0
			{
				solved = true;
				break;
			}
			if (ToExpand->size() > 1) //don't want to sort 1 element
			{ 
				sort(ToExpand->begin(), ToExpand->end(),
					[](const Point* a, const Point* b) //I could hide it or I could just admit that I took it from stack overflow 
													   //I should think more about lambda expressions, I also edited it to use pointers to my class and whatnot
				{										//I also added the 2nd expresion so that when estimated cost is equal lower moves value has a prefrence
					if (a->EstimatedCostToGoal != b->EstimatedCostToGoal)
						return (
							a->EstimatedCostToGoal > b->EstimatedCostToGoal
							);
					return(a->movesTillHere > b->movesTillHere);
				});
			}


		}
		else
		{ // no solution found

			break;

		}


	} while (true);
	
	//cout << CheckedStates << endl; //display amount of checked states for diagnostics
	//fill the Buffer 
	//would be good to make temporary buffer fills to ensure that the game doesn't lag

	Expanded.clear();
	if (solved) {
		for (unsigned int i = 0; i < (*ToExpand)[0]->moveListTillHere.size(); i++)
		{
			pOutBuffer[i] = (*ToExpand)[0]->moveListTillHere[i];
			//cout << pOutBuffer[i] << " ";  //display buffer to check if everything is ok
		}

		int a = (*ToExpand)[0]->movesTillHere;

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
		for (int k = ToExpand->size(); k > 1; k--) {
			delete (*ToExpand)[k - 1];
		}

		delete mapSearch;
		delete ToExpand;
		delete map;
		return -1;
	}


}


