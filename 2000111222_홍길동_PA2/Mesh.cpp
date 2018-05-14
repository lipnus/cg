#include "Mesh.h"

using namespace std;

void Mesh::LoadMesh(string str)
{
	ifstream file(str);
	string inputString;
	while (!file.eof())
	{
		getline(file, inputString);
		if (inputString[0] == '#')
			continue;
		string delimiter = " ";
		string tokens[4];
		size_t pos = 0;
		int index = 0;
		string token;
		while ((pos = inputString.find(delimiter)) != std::string::npos)
		{
			token = inputString.substr(0, pos);
			tokens[index] = token;
			inputString.erase(0, pos + delimiter.length());
			index++;
		}
		tokens[3] = inputString;
		if (tokens[0] == "v")
		{
			Vertex v;
			v.position = VECTOR3D(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
			vertexArray.push_back(v);
		}
		else if (tokens[0] == "f")
		{
			Face f;
			f.vertex0 = stoi(tokens[1]);
			f.vertex1 = stoi(tokens[2]);
			f.vertex2 = stoi(tokens[3]);
			f.vertex0--;
			f.vertex1--;
			f.vertex2--;
			faceArray.push_back(f);
		}
	}
	file.close();
}

void Mesh::ComputeFaceNormal()
{
	for (int i = 0; i < faceArray.size(); i++)
	{
		Face &f = faceArray[i];

		//Face에 연결된 정점들을 불러들임
		VECTOR3D v0 = vertexArray[f.vertex0].position;
		VECTOR3D v1 = vertexArray[f.vertex1].position;
		VECTOR3D v2 = vertexArray[f.vertex2].position;

		//삼각형의 정점들과 외적을 이용해 Normal계산
		VECTOR3D va = v1 - v0;
		VECTOR3D vb = v2 - v0;
		VECTOR3D vc = va.CrossProduct(vb);
		VECTOR3D vd = va.CrossProduct(vb);
		
		//법선 벡터는 방향 정보만 다루므로 Normalize
		vc.Normalize(); 
		f.normal = vc;
	}
}


//기준 Vertex를 포함하는 인접 Face를 찾는다
void Mesh::FindNeighborFaceArray()
{
	for (int i = 0; i < vertexArray.size(); i++)
	{
		for (int j = 0; j < faceArray.size(); j++)
		{
			//어떤 Vertex가 Face의 한 점이라면, 해당 Face를 그 Vertex의 NeighborFace로 봄
			if (faceArray[j].vertex0 == i || faceArray[j].vertex1 == i || faceArray[j].vertex2 == i)
			{
				vertexArray[i].neighborFaces.push_back(j);
			}
		}
	}
}

void Mesh::ComputeVertexNormal()
{
	for (int i = 0; i < vertexArray.size(); i++)
	{
		//Vertex Normal은 NeighborFace Normal들의 평균

		Vertex& v = vertexArray[i];
		for (int j = 0; j < vertexArray[i].neighborFaces.size(); j++)
		{
			v.normal += faceArray[vertexArray[i].neighborFaces[j]].normal;
		}
		v.normal /= vertexArray[i].neighborFaces.size();
	}
}