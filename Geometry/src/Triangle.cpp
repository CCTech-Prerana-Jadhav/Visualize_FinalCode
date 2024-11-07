#include <vector>
#include "Triangle.h"

Geometry::Triangle::Triangle(Geometry::Point normal, Geometry::Point p1, Geometry::Point p2, Geometry::Point p3)
    : p1(p1), p2(p2), p3(p3), normal(normal)
{
}

Geometry::Triangle::~Triangle()
{

}

Geometry::Point Geometry::Triangle::P1()
{
    return p1;
}

Geometry::Point Geometry::Triangle::P2()
{
    return p2;
}

Geometry::Point Geometry::Triangle::P3()
{
    return p3;
}

std::vector<Geometry::Point> Geometry::Triangle::Points() const
{
    std::vector<Point> points;
    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    return points;
}

Geometry::Point Geometry::Triangle::Normal() const
{
    return normal;
}


