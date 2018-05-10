#include <vector>

using namespace std;
class Point
{
public:
	vector<int> moveListTillHere;
	int prevPos;	
	int EstimatedCostToGoal; //mahatan heuristic
	int movesTillHere;
	int ID;
	Point();
	Point(int id);
	Point(vector<int> moveList,  int cTH,int id,int prevPost);
	
	
	
};
