#include "FinderHeader.h"
#include <algorithm>
#include <iostream>
using namespace std;
MapSearch::MapSearch() {



}
MapSearch::MapSearch(const int mapWidtht, const int mapHeightt, vector<int>* mapt, vector<Point*>* toExpandt, int goalIDt)
{
	goalID = goalIDt;
	mapWidth = mapWidtht;
	mapHeight = mapHeightt;
	map = mapt;
	toExpand = toExpandt;

}
void MapSearch::Expand(int * id, Point * Expand)
{	//This could've been done more neatly for sure
	 //I'm checking for things in a row
	 // first: am I expanding straight back to point where I came from(removin 25% of choice)
	 // 2 am I going to be out of bounds
	 //3 can I go there
	 //4 did I already expanded there (1 only check last move)




	if ( *id - 1 != Expand->prevPos &&
		(*id )%mapWidth != 0 &&
		(*map)[*id - 1] == 1  &&
		find_if(toExpand->begin(), toExpand->end(), [id](const Point * Pnt) {return Pnt->ID == *id - 1; }) == toExpand->end())
		{
		
			Point * p = new Point( Expand->moveListTillHere,Expand->movesTillHere, *id - 1,*id);
			CalculateCost(p);
			toExpand->push_back(p);
		}
	
		if (
			*id + 1 != Expand->prevPos && 
			((*id+1) % (mapWidth)!=0 ||*id == 0)&& 
			(*map)[*id + 1] == 1 && 
			find_if(toExpand->begin(), toExpand->end(), [id](const Point * Pnt) {return Pnt->ID == *id + 1; }) == toExpand->end())
		{
			Point * p = new Point(Expand->moveListTillHere, Expand->movesTillHere, *id + 1,*id);
			CalculateCost(p);
			toExpand->push_back(p);
		}
		int k = mapWidth;
		if (*id + mapWidth != Expand->prevPos && 
			*id  <= (mapHeight*mapWidth-mapWidth) &&
			(*map)[*id + mapWidth] == 1 && 
			find_if(toExpand->begin(), toExpand->end(), [id,k](const Point * Pnt) {return Pnt->ID == *id + k; }) == toExpand->end())
		{
			Point * p = new 	Point(Expand->moveListTillHere, Expand->movesTillHere, *id + mapWidth,*id);
			CalculateCost(p);
			toExpand->push_back(p);
		}

		if (*id - mapWidth != Expand->prevPos &&
			*id  >=mapWidth && 
			(*map)[*id - mapWidth] == 1 && 
			find_if(toExpand->begin(), toExpand->end(), [id,k](const Point * Pnt) {return Pnt->ID == *id - k; }) == toExpand->end())
		{
			Point * p = new 	Point(Expand->moveListTillHere, Expand->movesTillHere, *id - mapWidth,*id);
			CalculateCost(p);
			toExpand->push_back(p);
		}

	}

//Manhatan check
void MapSearch::CalculateCost( Point * p)
{
	// I'm extracting x and y from single ID number, that's why it looks a bit complicated
	//but basicly I'm just adding manhatan distance to amount of moves from start, so basiclaly a* I should also add some prefrence for distance but I wanted estimated distance to be an int
	//later this quantity is used to sort elements to be expanded
	if (p->ID != goalID)
	p->EstimatedDistanceToGoal = p->movesTillHere+abs((p->ID-(p->ID/mapWidth))- goalID - (goalID / mapWidth))+abs((p->ID / mapWidth)- (goalID / mapWidth));
	else {
		p->EstimatedDistanceToGoal = 0;
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
	vector<int>* map = new vector<int>() ; // Probably should've stayed with chars for map, but  for some reason int's look nicer to me, but than again memory is waisted
	
	for (int i = 0; i < nMapWidth*nMapHeight; i++)
	{
		unsigned char p = '\x1';
		
		if (*(pMap+i) == p)
			map->push_back(1);
		else
		{
			map->push_back(0);
		}
		
	}
	
	vector<Point*> * ToExpand = new vector<Point*>();
	MapSearch * mapSearch = new MapSearch(nMapWidth, nMapHeight,map, ToExpand, nTargetX+nTargetY*nMapWidth);
	ToExpand->push_back( new Point(nStartX+nMapWidth*nStartY));
	
	bool solved = false;
	//loop looks like this: expand points, sort them in order of distance to goal(witth heuristic)
	do
	{

		
			Point* tmp = ToExpand->back();
			//Expanded.push_back(tmp->ID);
			ToExpand->pop_back();//remember to ad destruction
			mapSearch->Expand( &(tmp->ID),tmp);
			if (ToExpand->size() != 0) {
			if ((*ToExpand)[0]->EstimatedDistanceToGoal == 0)
			{
				solved = true;
				break;
			}
			if (ToExpand->size() > 1) {
				sort(ToExpand->begin(), ToExpand->end(),
					[](const Point* a, const Point* b) //I could hide it or I could just admit that I took it from stack overflow 
													   //I should think more about lambda expressions, I also edited it to use pointers to my class and whatnot
				{
					return (a->EstimatedDistanceToGoal > b->EstimatedDistanceToGoal);//!!!!!!check if < shouldn't be other way
				});
			}


		}
		else
		{ // no solution found

			break;

		}


	} while (true);
	//fill the Buffer 
	//would be good to make temporary buffer fills to ensure that the game doesn't lag
	if (solved) {
		for (int i = 0; i < (*ToExpand)[0]->moveListTillHere.size(); i++)
		{
			pOutBuffer[i] = (*ToExpand)[0]->moveListTillHere[i];
		}
		return   (*ToExpand)[0]->movesTillHere;
	}
	else
	{

		return -1;
	}


}


