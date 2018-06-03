#pragma once


#include <vector>
# include "Point.h"


class MapSearch
{
private:
	int mapWidth;
	int mapHeight;
	int goalID;
	std::vector<char>* map;
	std::vector<Point*>* toExpand;
	//vector<int>* Expanded;
public:


	MapSearch();
	~MapSearch();
	MapSearch(const int  mapWidtht, const int mapHeightt, std::vector<char>* mapt, std::vector<Point*>* toExpandt, int goalIDt);
	void Insert(Point* pnt);

	void Expand(int * id, Point * Expand, std::vector<int>* Expanded);

	void CalculateCost(Point * p);

};

int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize);
