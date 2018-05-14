#pragma once

#include "VECTOR.h"
#include <vector>

using namespace std;

class Vertex
{
public:
	VECTOR3D position;
	VECTOR3D normal;

	vector<int> neighborFaces;
};