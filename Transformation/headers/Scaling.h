#pragma once
#include "Triangulation.h"

namespace Transformation
{
	class Scaling
	{
		Scaling();
		~Scaling();
		void scale(Geometry::Triangulation&, Geometry::Triangulation&,double,double,double);
	};
}