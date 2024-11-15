#include "Matrix4x4.h"
using namespace Geometry;
Matrix4x4::Matrix4x4() :matrix({ {1.0,0.0,0.0,0.0}, {0.0,1.0,0.0,0.0} ,{0.0,0.0,1.0,0.0} ,{0.0,0.0,0.0,1.0} })
{
}
Matrix4x4::~Matrix4x4()
{
}


std::vector<double> Matrix4x4::multiply(double x, double y, double z)
{
	std::vector<double> matrix2{ x,y,z,1 };
	std::vector<double> ans(4,0.0);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans[i] += matrix[i][j] * matrix2[j];
		}
	}
	return ans;
}

