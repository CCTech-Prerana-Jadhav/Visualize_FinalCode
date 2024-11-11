#pragma once
#include <Triangulation.h>
#include <map>


using namespace Geometry;

namespace Transform {
	class Transformation {
	public:
		Transformation();
		~Transformation();
		void scale(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double scalingFactor);
		void processPoint(double value, Triangulation& outputTriangulation, std::map<double, int, Transformation>& uniqueValueMap, std::vector<int>& pointIndices);
		void rotate(Triangulation& inputTriangulation, Triangulation& outputTriangulation, double angle, int axis[3]);
		void translate();
		bool operator()(double a, double b) const;
	};
}

