#include <iostream>
#include "FinderHeader.h"
#include <vld.h>
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
		1,1,1,1,1,1,1,1,1,1 ,
	1,1,1,1,1,1,1,1,1,1 };
	int poutbuffer[50];
	
	int lPath = FindPath(0, 0, 9, 9, pmap, 10, 10, poutbuffer, 50);
	cout << lPath << endl;
	

}

