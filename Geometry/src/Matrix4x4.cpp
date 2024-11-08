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

}
