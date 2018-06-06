//Main comment in FindPath.cpp

#include <iostream>
#include "FindPath.h"
#include <chrono>

//#include <vld.h>  // memory leak thingy I used
using namespace std;
int main()
{

	unsigned char pmap[10000];
	//map generation

	for (int i = 0; i < 10000; i++) {

		if (i / 100 > 10 && i / 100 < 95) {
			if (i % 7 == 5)
				pmap[i] = 0;
			else if ((i / 100) % 9 == 2 && i % 100 < 96)
				pmap[i] = 0;
			else if ((i / 100) % 9 == 7 && i % 100 > 13)
				pmap[i] = 0;
			else
				pmap[i] = 1;
		}
		else
			pmap[i] = 1;
	}


	const int bufferSize = 50;
	int poutbuffer[bufferSize];
	const int dimx = 100;
	const int dimy = 100;
	auto start = chrono::high_resolution_clock::now();
	int lPath = FindPath(99, 99, 0, 0, pmap, dimx, dimy, poutbuffer, bufferSize);

	auto end = chrono::high_resolution_clock::now();
	chrono::duration<float> duration = end - start;



	//drawing map with obtained path, map is reflected through x axis
	//P-path
	//_ - possible to go
	//| - wall
	cout << "---------------" << endl;
	for (int z = 0; z < dimy; z++)
	{
		for (int x = 0; x < dimx; x++) {
			bool p = false;
			for (int i : poutbuffer)
			{

				if (dimx * z + x == i&&i>=1) {
					cout << "P ";
					p = true;
				}
			}
			if (!p) {
				if (pmap[dimx * z + x] == '\x1')
					cout << "_ ";//
				else
					cout << "| ";
			}
		}
		cout << endl;
	}

	cout << "Moves num: " << lPath << endl;
	cout << "Time to find Path: " << duration.count() << endl;
	cin.get();

}




