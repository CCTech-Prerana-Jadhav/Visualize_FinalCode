#pragma once
#include "Triangulation.h"
#include "Matrix4x4.h"
namespace Transformation
{
	using namespace Geometry;
	class Transformation 
	{
	public:
		bool operator()(double a, double b) const;
		Triangulation scaling(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double scalex = 2.0, double scaley = 2.0, double scalez = 1.0);
	};
}