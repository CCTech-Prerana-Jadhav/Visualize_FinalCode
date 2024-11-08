#pragma once
#include <Triangulation.h>

using namespace Geometry;

namespace Transformation {
	class Transform {
		Transform();
		~Transform();
		void scale(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double scalingFactor);
		void rotate();
		void translate();
	};
}