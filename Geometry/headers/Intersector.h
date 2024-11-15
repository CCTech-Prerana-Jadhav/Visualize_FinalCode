#include "Triangulation.h"
using namespace Geometry;
using namespace std;

class Intersector
{
public:
	double y;
	Triangle t;

	Point* isIntersecting(Point& p1, Point& p2, double y, const vector<double>& uniqueValues);
	vector<Point> intersect(const Triangle& t, double y, const Triangulation&);

};