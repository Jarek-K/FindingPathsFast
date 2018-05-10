#pragma once
#include <vector>
# include "Point.h"

using namespace std;
class MapSearch
{
private:
	 int mapWidth;
	 int mapHeight;
	 int goalID;
	vector<int>* map;
	vector<Point*>* toExpand;
	//vector<int>* Expanded;
	public:
	

	MapSearch();
	MapSearch(const int  mapWidtht, const int mapHeightt, vector<int>* mapt, vector<Point*>* toExpandt, int goalIDt );

	
	void Expand(int * id, Point * Expand);
	void CalculateCost( Point * p);
	
};
int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize);
