#include "MeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Renderer.h"
#define FACE_ELEMENTS 3


using namespace std; 
extern float tx,ty,tz,sx, sy, sz, scaler,theta_x,theta_y,theta_z;
extern bool setWorldTransform;

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
			p *= 0.7;
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

void MeshModel::createTransformation()
{
	glm::mat4x4 scale = glm::mat4x4(sx*scaler, 0, 0, 0,
									0, sy*scaler, 0, 0,
									0, 0, sz*scaler, 0,
									0, 0, 0, 1);
	
	glm::mat4x4 rotX = glm::mat4x4(1,		0,			 0,			 0,
									0, cos(theta_x), -sin(theta_x),  0,
									0, sin(theta_x), cos(theta_x),   0,
									0,		0,  		 0,			 1);

	glm::mat4x4 rotY = glm::mat4x4(cos(theta_y), 0, sin(theta_y), 0,
									0,			 1,		0,		  0,
								  -sin(theta_y), 0, cos(theta_y), 0,
									0,			 0,		0,		  1);

	glm::mat4x4 rotZ = glm::mat4x4(cos(theta_z), -sin(theta_z), 0, 0,
							    	sin(theta_z), cos(theta_z), 0, 0,
									0,				 0,			1, 0,
									0,				 0,			0, 1);
	
	glm::mat4x4 translate = glm::transpose(glm::mat4x4({ 1, 0 ,0, tx },
													   { 0, 1, 0, ty },
													   { 0, 0, 1, tz },
													   { 0, 0, 0, 1 }));
	
	if(setWorldTransform)
		worldTransform = scale * rot*translate;			// rot =??? X? y? z?
	else
		objTransform = scale * rot*translate;			// rot = ???.........
}

const vector<glm::vec4>* MeshModel::Draw()
{
	//Returns the points to draw the meshmodel
	for (int i = 0; i < vertexPositions->size(); i++)
	{
		auto p = (*vertexPositions)[i];
		auto q = glm::vec4(p.x, p.y, p.z, 1);
		(*vertexPositions_transformed)[i] = worldTransform * objTransform*q;
	}
	return vertexPositions_transformed;
}
