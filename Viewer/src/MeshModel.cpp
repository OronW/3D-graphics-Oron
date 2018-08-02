#include "MeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#define FACE_ELEMENTS 3


using namespace std; 

// A struct for processing a single line in a wafefront obj file:
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
struct FaceIdx
{
	// For each of the following 
	// Saves vertex indices
	int v[FACE_ELEMENTS];
	// Saves vertex normal indices
	int vn[FACE_ELEMENTS];
	// Saves vertex texture indices
	int vt[FACE_ELEMENTS];

	FaceIdx()
	{
		for (int i = 0; i < FACE_ELEMENTS + 1; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdx(std::istream& issLine)
	{
		for (int i = 0; i < FACE_ELEMENTS + 1; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < FACE_ELEMENTS; i++)
		{
			issLine >> std::ws >> v[i] >> std::ws;
			if (issLine.peek() != '/')
			{
				continue;
			}
			issLine >> c >> std::ws;
			if (issLine.peek() == '/')
			{
				issLine >> c >> std::ws >> vn[i];
				continue;
			}
			else
			{
				issLine >> vt[i];
			}
			if (issLine.peek() != '/')
			{
				continue;
			}
			issLine >> c >> vn[i];
		}
	}
};

glm::vec3 vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel::MeshModel()
{
	//this.renderer = renderer;
}

MeshModel::MeshModel(const string& fileName)
{
	LoadFile(fileName);
}

MeshModel::~MeshModel()
{
}

void MeshModel::LoadFile(const string& fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdx> faces;
	vector<glm::vec3> vertices;
	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read the type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") /*BUG*/
		{
			glm::vec3 p = vec3fFromStream(issLine);
			p *= 0.1;
			vertices.push_back(p);
		}
		else if (lineType == "f") /*BUG*/
		{
			faces.push_back(issLine);
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertexPositions should contain:
	//vertexPositions={v1,v2,v3,v1,v3,v4}

	vertexPositions = new vector<glm::vec3>; /*BUG*/
	// iterate through all stored faces and create triangles
	int k=0;
	//for(vector<FaceIdx>::iterator it = faces.begin(); it != faces.end(); ++it)
	for(FaceIdx current_face: faces)
	{
		for (int i = 0; i < FACE_ELEMENTS; i++)
		{
			int vertex_index = current_face.v[i]-1;	//obj file starts with vertex index 1
			vertexPositions->push_back(vertices[vertex_index]); /*BUG*/
		}
	}
}

const vector<glm::vec3>* MeshModel::Draw()
{
	//Returns the points to draw the meshmodel
	return vertexPositions;
}
