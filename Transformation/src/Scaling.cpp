#include "Scaling.h"
#include "Triangle.h"

Transformation::Scaling::Scaling()
{

}

Transformation::Scaling::~Scaling()
{

}

void Transformation::Scaling::scale(Geometry::Triangulation& inTriangulation, Geometry::Triangulation& outTriangulation, double scaleX,double scaleY,double scaleZ)
{
	
	for (Geometry::Triangle tri : inTriangulation.Triangles) 
	{

	}
}
