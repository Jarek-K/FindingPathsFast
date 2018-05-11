//Main comment in FindPath.cpp

#include <iostream>
#include "FinderHeader.h"
//#include <vld.h>  // memory leak thingy I used
using namespace std;
int main()
{
	unsigned char pmap[] = { 1,1,1,1,1,1,1,1,1,1,
		1,1,1,1,1,1,1,1,1,1 ,
		0,1,1,1,1,1,0,0,1,1 ,
		1,0,1,1,1,0,1,1,1,1 ,
		1,1,0,1,0,1,1,0,0,0 ,
		1,1,1,0,1,1,1,1,1,1 ,
		1,1,1,1,1,1,1,1,1,1 ,
		1,1,1,1,1,1,1,1,1,1 ,
		1,1,1,1,0,0,0,0,0,0 ,
	1,1,1,1,1,1,1,1,1,1 };
	const int bufferSize = 30;
	int poutbuffer[bufferSize];

	int lPath = FindPath(0, 0, 9, 9, pmap, 10, 10, poutbuffer, bufferSize);

	cout << lPath << endl;


}

