#pragma once
#include <Triangulation.h>

using namespace Geometry;

namespace Transform {
	class Transformation {
		Transformation();
		~Transformation();
		void scale(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double scalingFactor);
		void rotate();
		void translate();
	};
}