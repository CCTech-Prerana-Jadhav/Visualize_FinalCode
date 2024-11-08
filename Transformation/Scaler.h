#pragma once
#include <Triangulation.h> ;
using namespace Geometry;
namespace Transformation {
	class Scaler {

	public :
		Scaler();
		~Scaler();

		void Scale(Triangulation& inputTriangulation, Triangulation& onputTriangulation, float xS = 1, float yS = 1,float zS = 1);

	};

}
