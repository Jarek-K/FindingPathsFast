#include <iostream>


#include "FinderHeader.h"

using namespace std;
int main()
{
	unsigned char pMap[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
	int pOutBuffer[7];
	
	int lPath = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
	cout << lPath << endl;


}

