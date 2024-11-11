#pragma once
#include <vector>
using namespace std;
namespace Geometry {
	class Matrix4x4
	{
	public:
		vector<vector<double>> matrix;

		Matrix4x4();
		~Matrix4x4();
		vector<double> multiply(vector<double>& point);
	};
}
