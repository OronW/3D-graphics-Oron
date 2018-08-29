#pragma once
#include "ImguiMenus.h"
#include <stdio.h>
#include <stdlib.h>
// open file dialog cross platform https://github.com/mlabbe/nativefiledialog
#include <nfd.h>
#include "Scene.h"
bool setWorldTransform = false, showNormals = false;
bool rotateX = false, rotateY = false, rotateZ = false;
bool rotatebytheta = false, translating = false;
float theta_x =0.0f, theta_y=0.0f, theta_z = 0.0f;
float sx = 1.0f, sy = 1.0f, sz = 1.0f, scaler = 1.0f;
float tx = 0, ty = 0,  tz=0;
bool sx_original = false, sy_original = false, sxy_original = false, scaling = false;

bool showDemoWindow = false;
bool showAnotherWindow = false;
bool showFile = false;
glm::vec4 clearColor = glm::vec4(0.4f, 0.55f, 0.60f, 1.00f);
glm::vec3 ObjColor = glm::vec3(1.0f, 0.0f, 1.0f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene* scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Test Menu");
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
		ImGui::SliderFloat("float", &sx, 0.0f, 3.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color
		
		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Begin("Our New Menu");
		ImGui::Checkbox("Scaling", &scaling);
		ImGui::Checkbox("rotating", &rotatebytheta);
		ImGui::Checkbox("Translating", &translating);
		ImGui::Checkbox("SetWorldTransform", &setWorldTransform);
		ImGui::Checkbox("Show normals", &showNormals);
		ImGui::ColorEdit3("OBJ color", (float*)&ObjColor);
		ImGui::End();

	}

	if (translating)
	{
		ImGui::Begin("Translating");
		ImGui::SliderFloat("x translate value ", &tx, tx-0.5f, tx+0.5f);
		ImGui::SliderFloat("y translate value ", &ty, ty-0.5f, ty+0.5f);
		ImGui::SliderFloat("z translate value ", &tz, tz-0.5f, tz+0.5f);
		if (ImGui::Button("Close Me"))
			translating = false;
		ImGui::End();
	}
	if (scaling)
	{
		
		ImGui::Begin("scaling");
		ImGui::SliderFloat("x scale value ", &sx, sx + 0.5f, sx -0.5f);
		ImGui::SameLine();
		ImGui::InputFloat(" X scale", &sx, 1.00f, 20.0f); 
		
		ImGui::SliderFloat("y scale value ", &sy, sy + 0.5f, sy - 0.5f);
		ImGui::SameLine();
		ImGui::InputFloat(" Y scale", &sy, 1.00f, 20.0f);
		
		ImGui::SliderFloat("z scale value ", &sz, sz + 0.5f, sz - 0.5f);
		ImGui::SameLine();
		ImGui::InputFloat(" Z scale", &sz, 1.00f, 20.0f);
		
		ImGui::SliderFloat("locked scale value ", &scaler, scaler + 0.5f, scaler - 0.5f);
		ImGui::SameLine();
		ImGui::InputFloat(" X&Y scale", &sy, 1.00f, 20.0f);
				
		if (ImGui::Button("ResetX"))
			sx = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("ResetY"))
			sy = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("ResetZ"))
			sz = 1.0f;
		ImGui::SameLine();
		if (ImGui::Button("ResetX&Y"))
			scaler = 1.0f;
		if (ImGui::Button("Close Me"))
			scaling = false;
		ImGui::End();

	}

	if (rotatebytheta)
	{
		ImGui::Begin("Rotating");
		ImGui::SliderFloat("rotate by X", &theta_x, theta_x-0.5f, theta_x+0.5f);
		ImGui::SameLine();
		if (ImGui::Button("Reset X"))
			theta_x = 0.0f;
		ImGui::SliderFloat("rotate by Y", &theta_y, theta_y-0.5f, theta_y+0.5f);
		ImGui::SameLine();
		if (ImGui::Button("Reset Y"))
			theta_y = 0.0f;
		ImGui::SliderFloat("rotate by Z", &theta_z, theta_z - 0.5f, theta_z + 0.5f);
		ImGui::SameLine();
		if (ImGui::Button("Reset Z"))
			theta_z = 0.0f;
		if (ImGui::Button("Close Me"))
			rotatebytheta = false;
		ImGui::End();
	}
	
	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (showAnotherWindow)
	{
		int val[2]; val[0] = io.MousePos.x; val[1] = io.MousePos.y;
		ImGui::Begin("Another Window", &showAnotherWindow);
		ImGui::InputInt2("(x,y)", val, 3);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (showDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						scene->LoadOBJModel(outPath);
						ImGui::Text("Hello from another window!");
						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}
}