#include "Transformation.h"
#include <map>
#define TOLERANCE 0.0000001

using namespace Geometry;

bool Transformation::Transformation::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}

Triangulation Transformation::Transformation::scaling(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double scalex, double scaley, double scalez)
{
    std::vector<double> nmatrix{0.0,0.0,0.0,0.0};
    std::vector<Point> vertices;
    matrix.setval(scalex, 0, 0);
    matrix.setval(scaley, 1, 1);
    matrix.setval(scalez, 2, 2);
    std::map<double, int, Transformation> uniqueMap;
    Triangulation newtriangulation;

    for (Triangle tri : triangulation.Triangles)
    {
        std::vector<Point> tempvect;
        tempvect = tri.Points();
        tempvect.push_back(tri.Normal());
        for (Point pt : tempvect)
        {
            nmatrix = matrix.multiply(triangulation.UniqueNumbers[pt.X()], triangulation.UniqueNumbers[pt.Y()], triangulation.UniqueNumbers[pt.Z()]);
            int pts[3];
            for (int i = 0; i < 3; i++)
            {
                auto pair = uniqueMap.find(nmatrix[i]);
                if (pair == uniqueMap.end())
                {
                    newtriangulation.UniqueNumbers.push_back(nmatrix[i]);
                    uniqueMap[nmatrix[i]] = newtriangulation.UniqueNumbers.size() - 1;
                    pts[i] = newtriangulation.UniqueNumbers.size() - 1;

                }
                else
                {
                    pts[i] = pair->second;

                }
            }
            vertices.push_back(Point(pts[0], pts[1], pts[2]));
        }
        newtriangulation.Triangles.push_back(Triangle(vertices[3],vertices[0], vertices[1], vertices[2]));
        vertices.clear();
    }
    return newtriangulation;
}



//double xyz[3]; // To store 3 coordinates as double
//std::vector<int> pointIndices; // To store indices of unique double values
//
//std::ifstream infile(fileName);
//if (infile.is_open())
//{
//    std::string line;
//    while (getline(infile, line))
//    {
//        std::stringstream ss(line);
//        std::string word;
//
//        while (ss >> word)
//        {
//            if (word == "vertex" || word == "normal")
//            {
//                ss >> xyz[0] >> xyz[1] >> xyz[2];
//
//                for (int i = 0; i < 3; i++)
//                {
//                    auto pair = uniqueValueMap.find(xyz[i]);
//                    if (pair == uniqueValueMap.end())
//                    {
//                        triangulation.UniqueNumbers.push_back(xyz[i]);
//                        uniqueValueMap[xyz[i]] = triangulation.UniqueNumbers.size() - 1;
//                        pointIndices.push_back(triangulation.UniqueNumbers.size() - 1);
//                    }
//                    else
//                    {
//                        pointIndices.push_back(pair->second);
//                    }
//                }
//            }
//
//            if (pointIndices.size() == 12)
//            {
//                Point normal = Point(pointIndices[0], pointIndices[1], pointIndices[2]);
//                Point p1 = Point(pointIndices[3], pointIndices[4], pointIndices[5]);
//                Point p2 = Point(pointIndices[6], pointIndices[7], pointIndices[8]);
//                Point p3 = Point(pointIndices[9], pointIndices[10], pointIndices[11]);
//                Triangle t(normal, p1, p2, p3);
//                triangulation.Triangles.push_back(t);
//                pointIndices.clear();
//            }
//        }
//    }
//}