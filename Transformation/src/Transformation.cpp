#include "Transformation.h"
#include "Matrix4x4.h"
#include <vector>
#include <map>

using namespace Transform;

Transformation::Transformation()
{
};

Transformation::~Transformation()
{

}

void Transformation::scale(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double scalingFactor) 
{
	Matrix4x4 matrix4x4;
	std::vector<double> point;
	std::map<double, int> uniqueValueMap;
	std::vector<int> pointIndices;
	matrix4x4.matrix = { {scalingFactor,0,0,0},
		{0,scalingFactor,0,0}, 
		{0,0,scalingFactor,0 },
		{0,0,0,1}};

	for (Triangle tri : inputTriangulation.Triangles) {
		for (Point p : tri.Points()) {
			point.push_back(inputTriangulation.UniqueNumbers[p.X()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Y()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Z()]);
			point.push_back(1.0);
			matrix4x4.multiply(point);
			for (int i = 0; i < 3; i++)
			{
				auto pair = uniqueValueMap.find(point[i]);
				if (pair == uniqueValueMap.end())
				{
					outputTriangulation.UniqueNumbers.push_back(point[i]);
					uniqueValueMap[point[i]] = outputTriangulation.UniqueNumbers.size() - 1;
					pointIndices.push_back(outputTriangulation.UniqueNumbers.size() - 1);
				}
				else
				{
					pointIndices.push_back(pair->second);
				}
			}
			if (pointIndices.size() == 12)
			{
				Point p1 = Point(pointIndices[0], pointIndices[1], pointIndices[2]);
				Point p2 = Point(pointIndices[3], pointIndices[4], pointIndices[5]);
				Point p3 = Point(pointIndices[6], pointIndices[7], pointIndices[8]);
				Triangle t(tri.Normal(), p1, p2, p3);
				outputTriangulation.Triangles.push_back(t);
				pointIndices.clear();
			}
			point.clear();
		}
	}
}

void Transform::Transformation::rotate()
{
}

void Transform::Transformation::translate()
{
}
