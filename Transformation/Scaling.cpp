#include "Scaling.h"
#include "Matrix4X4.h"

void Scaling::uniformScaling(Triangulation& triangulation, float scalingFactor)
{
	scalingMatrix = { {scalingFactor,0.0,0.0,0.0} ,{0.0,scalingFactor,0.0,0.0} ,{0.0,0.0,scalingFactor,0.0} ,{0.0,0.0,0.0,1.0} };
	//Matrix multiplication
	//Geometry::Matrix4x4::matrixMultiplication(scalingMatrix,vector<double>Point);
}

void Scaling::nonUniformScaling(Triangulation& triangulation, vector<float> scalingFactor)
{
	float x, y, z;
	scalingFactor = {x,y,z};
	scalingMatrix = { {scalingFactor[0],0.0,0.0,0.0} ,{0.0,scalingFactor[1],0.0,0.0} ,{0.0,0.0,scalingFactor[2],0.0} ,{0.0,0.0,0.0,1.0}};
	//Matrix multiplication
}
