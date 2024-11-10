#include "Transformation.h"
#include "Matrix4x4.h"
#include <vector>
#include <cmath>

#define TOLERANCE 0.0000001

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
	std::map<double, int, Transformation> uniqueValueMap;
	std::vector<int> pointIndices;
	matrix4x4.matrix = { {scalingFactor,0,0,0},
		{0,scalingFactor,0,0},
		{0,0,scalingFactor,0 },
		{0,0,0,1} };

	for (Triangle tri : inputTriangulation.Triangles) {
		for (Point p : tri.Points()) {
			point.push_back(inputTriangulation.UniqueNumbers[p.X()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Y()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Z()]);
			point.push_back(1.0);
			matrix4x4.multiply(point);
			for (int i = 0; i < 3; i++)
			{
				processPoint(point[i], outputTriangulation, uniqueValueMap, pointIndices);
			}
			point.clear();
		}
		Point p = tri.Normal();
		point.push_back(inputTriangulation.UniqueNumbers[p.X()]);
		point.push_back(inputTriangulation.UniqueNumbers[p.Y()]);
		point.push_back(inputTriangulation.UniqueNumbers[p.Z()]);
		for (int i = 0; i < 3; i++)
		{
			processPoint(point[i], outputTriangulation, uniqueValueMap, pointIndices);
		}
		point.clear();
		if (pointIndices.size() == 12)
		{
			Point p1 = Point(pointIndices[0], pointIndices[1], pointIndices[2]);
			Point p2 = Point(pointIndices[3], pointIndices[4], pointIndices[5]);
			Point p3 = Point(pointIndices[6], pointIndices[7], pointIndices[8]);
			Point normal = Point(pointIndices[9], pointIndices[10], pointIndices[11]);
			Triangle t(normal, p1, p2, p3);
			outputTriangulation.Triangles.push_back(t);
			pointIndices.clear();
		}
	}
}

void Transform::Transformation::translate()
{

}

bool Transformation::operator()(double a, double b) const
{
	return fabs(a - b) > TOLERANCE ? a < b : false;
}

void Transformation::processPoint(double value, Triangulation& outputTriangulation, std::map<double, int, Transformation>& uniqueValueMap, std::vector<int>& pointIndices)
{
	auto pair = uniqueValueMap.find(value);
	if (pair == uniqueValueMap.end())
	{
		outputTriangulation.UniqueNumbers.push_back(value);
		uniqueValueMap[value] = outputTriangulation.UniqueNumbers.size() - 1;
		pointIndices.push_back(outputTriangulation.UniqueNumbers.size() - 1);
	}
	else
	{
		pointIndices.push_back(pair->second);
	}
}

void Transform::Transformation::rotate(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double angle, int axis[3])
{
	double radian = angle * 0.01745329;
	Matrix4x4 matrix4x4;
	std::vector<double> point;
	std::map<double, int, Transformation> uniqueValueMap;
	std::vector<int> pointIndices;
	if (axis[0] == 1) {
		matrix4x4.matrix = { {1,0,0,0},
			{0,cos(radian),-sin(radian),0},
			{0,sin(radian),cos(radian),0},
			{0,0,0,1} };
	}
	for (Triangle tri : inputTriangulation.Triangles) {
		for (Point p : tri.Points()) {
			point.push_back(inputTriangulation.UniqueNumbers[p.X()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Y()]);
			point.push_back(inputTriangulation.UniqueNumbers[p.Z()]);
			point.push_back(1.0);
			matrix4x4.multiply(point);
			for (int i = 0; i < 3; i++)
			{
				processPoint(point[i], outputTriangulation, uniqueValueMap, pointIndices);
			}
			point.clear();
		}
		Point p = tri.Normal();
		point.push_back(inputTriangulation.UniqueNumbers[p.X()]);
		point.push_back(inputTriangulation.UniqueNumbers[p.Y()]);
		point.push_back(inputTriangulation.UniqueNumbers[p.Z()]);
		for (int i = 0; i < 3; i++)
		{
			processPoint(point[i], outputTriangulation, uniqueValueMap, pointIndices);
		}
		point.clear();
		if (pointIndices.size() == 12)
		{
			Point p1 = Point(pointIndices[0], pointIndices[1], pointIndices[2]);
			Point p2 = Point(pointIndices[3], pointIndices[4], pointIndices[5]);
			Point p3 = Point(pointIndices[6], pointIndices[7], pointIndices[8]);
			Point normal = Point(pointIndices[9], pointIndices[10], pointIndices[11]);
			Triangle t(normal, p1, p2, p3);
			outputTriangulation.Triangles.push_back(t);
			pointIndices.clear();
		}
	}
}
