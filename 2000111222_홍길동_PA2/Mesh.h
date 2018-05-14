#pragma once

#include "Vertex.h"
#include "Face.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Mesh
{
public:
	vector<Vertex>	vertexArray;
	vector<Face>	faceArray;

	void			LoadMesh(string str);
	void			ComputeFaceNormal();
	void			FindNeighborFaceArray();
	void			ComputeVertexNormal();
};