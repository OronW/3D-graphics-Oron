#include "Scene.h"
#include "MeshModel.h"
#include <string>

using namespace std;
void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	model->createTransformation();
	models.push_back(model);
}

void Scene::Draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves
	renderer->SetBuffer();

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->createTransformation();
		//const std::vector<glm::vec4>* tris = models[i]->Draw();
		//renderer->DrawTriangles(tris);
		renderer->Render(models[i]->vbo_vertices, models[i]->verticesSize, models[i]->objTransform);
	}

	renderer->SwapBuffers();
}

void Scene::DrawDemo()
{
	renderer->SetDemoBuffer();
	renderer->SwapBuffers();
}
