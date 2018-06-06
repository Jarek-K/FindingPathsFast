
#include <vector>


class Point
{
public:
	std::vector<int> moveListTillHere;
	Point * prev;
	int prevPos;
	int EstimatedCostToGoal; //mahatan heuristic
	int movesTillHere;
	int ID;
	Point();

	Point(int id);
	Point(Point * prevt, int cTH, int id);

};
