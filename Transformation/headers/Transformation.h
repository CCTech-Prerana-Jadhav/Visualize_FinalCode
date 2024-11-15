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
		Triangulation scaling(Triangulation& triangulation, double scalex = 0.5, double scaley = 0.5, double scalez = 0.5);
		Triangulation Translatate(Triangulation& triangulation, double tx = 10, double ty = 10, double tz = 1);
	};
}