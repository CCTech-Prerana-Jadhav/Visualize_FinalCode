#include "Matrix4x4.h"
using namespace Geometry;

Geometry::Matrix4x4::Matrix4x4()
{
    matrix = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };
}

Geometry::Matrix4x4::~Matrix4x4()
{
}

void Geometry::Matrix4x4::multiply(vector<double>& point)
{
    vector<double> result(4, 0); 
    for (int i = 0; i < 4; ++i) {
        double temp = 0;
        for (int j = 0; j < 4; ++j) {
            temp += matrix[i][j] * point[j];
        }
        result[i] = temp; 
    }
    point = result;
}
