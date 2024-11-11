#include "Matrix4x4.h"
using namespace Geometry;


Geometry::Matrix4x4::Matrix4x4()
{
	matrix = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{0,0,0,1}
	};
}

Geometry::Matrix4x4::~Matrix4x4()
{ 
    
}

vector<double> Geometry::Matrix4x4::multiply(vector<double>& point)
{
	vector<double> result = {0,0,0,0};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
			result[i] += matrix[i][j] * point[j];
        }
    }
	return result;
}
