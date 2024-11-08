#pragma once
#include <Triangulation.h>
using namespace Geometry;
namespace Transformation {
	class Transform {

	public:
		Transform();
		~Transform();

		/*void Scale(Triangulation& inputTriangulation, Triangulation& onputTriangulation, float xS = 1, float yS = 1, float zS = 1);*/
		void Scale(Triangulation& inputTriangulation, Triangulation& outputTriangulation, float Scalingfactor = 1);
		void rotate();
		void translate();
	};

}

