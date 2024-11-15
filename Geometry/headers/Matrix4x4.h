#pragma once
#include <vector>
#include "Point.h"
namespace Geometry 
{
	class Matrix4x4 
	{
	public:
		std::vector<std::vector<double>> matrix;
		Matrix4x4();
		~Matrix4x4();
		std::vector<double> multiply(double x,double y,double z);
		//void setval(double value,int x,int y);
	};

}
