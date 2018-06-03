
#include <vector>


class Point
{
public:
	std::vector<int> moveListTillHere;

	int prevPos;
	int EstimatedCostToGoal; //mahatan heuristic
	int movesTillHere;
	int ID;
	Point();

	Point(int id);
	Point(std::vector<int> moveList, int cTH, int id);

};
