#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Renderer.h"
#include "MeshModel.h"
#include "Light.h"
#include "Camera.h"

using namespace std;

class Scene {
private:
	vector<MeshModel*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *renderer;

public:
	Scene() {};
	Scene(Renderer *renderer) : renderer(renderer) {};

	// Loads an obj file into the scene.
	void LoadOBJModel(string fileName);

	// Draws the current scene.
	void Draw();

	// Draws an example.
	void DrawDemo();
	
	int ActiveModel;
	int ActiveLight;
	int ActiveCamera;
};