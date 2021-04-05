#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../display/display.h"
#include "../renderObject/renderObject.h"
#include "../shader/shader.h"
#include <iostream>
#include <vector>
class gui
{
public:
	void init(display* display_)
	{
		m_id = rand();
		m_display = display_;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsClassic();
		ImGui_ImplGlfw_InitForOpenGL(m_display->getGLFWWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		isVisible = false;
		m_showSceneInformations = false;
		m_showRenderingDebug = false;
	}
	void setVisibility(bool isVisible_) { isVisible = isVisible_; }
	void update(vector<renderObject*>* objectHolder, vector<light*> lights, GLuint albedoSpecTexture, GLuint normalTexture, GLuint positionTexture, GLuint roughnessTexture, GLuint metallicTexture)
	{
		if(isVisible)
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
			
			showWelcome();

			if (m_showRenderingDebug)
			{
				showRenderingDebug(positionTexture, normalTexture, albedoSpecTexture, roughnessTexture, metallicTexture);
			}
			if (m_showSceneInformations)
			{
				showSceneInformations(objectHolder);
			}

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Rendering debug"))
				{
					m_showRenderingDebug = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene informations"))
				{
					m_showSceneInformations = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Quit"))
				{
					
				}
				ImGui::EndMainMenuBar();
			}


			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
private:
	int m_id;
	display* m_display;
	bool isVisible;

	bool m_showRenderingDebug, m_showSceneInformations;

	void showRenderingDebug(GLuint positionTexture, GLuint normalTexture, GLuint albedoSpecTexture, GLuint roughnessTexture, GLuint metallicTexture)
	{
		ImGui::Begin("Deferred rendering debug", &isVisible);
		ImGui::SetWindowFontScale(1.1);

		ImGui::TextWrapped("Position: ");
		IM_ASSERT(positionTexture);
		ImGui::Image((void*)(intptr_t)positionTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		ImGui::TextWrapped("Normals: ");
		IM_ASSERT(normalTexture);
		ImGui::Image((void*)(intptr_t)normalTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		ImGui::TextWrapped("Albedo: ");
		IM_ASSERT(albedoSpecTexture);
		ImGui::Image((void*)(intptr_t)albedoSpecTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		ImGui::TextWrapped("Roughness: ");
		IM_ASSERT(roughnessTexture);
		ImGui::Image((void*)(intptr_t)roughnessTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::Separator();

		ImGui::TextWrapped("Metallic: ");
		IM_ASSERT(metallicTexture);
		ImGui::Image((void*)(intptr_t)metallicTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}

	void showWelcome()
	{
		//Welcome
		ImGui::Begin("Hello", &isVisible);
		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welome to TITAN Engine !");
		ImGui::TextWrapped("Keep in mind : This engine is still in early development !");
		ImGui::TextWrapped("The 'scene informations' panel only prints active objects");
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "From github.com/leosncz/titan");
		ImGui::End();
	}

	void showSceneInformations(vector<renderObject*>* objectHolder)
	{
		//Scene information
		ImGui::Begin("Scene informations", &isVisible);
		ImGui::SetWindowFontScale(1.1);
		for (int i = 0; i < objectHolder->size(); i++)
		{
			vector<renderObject*>* test = objectHolder;
			renderObject* object = test->at(i);
			string name = "ID : ";
			name.append(to_string(object->getID()));
			ImGui::Text(name.c_str());
			name = "TAG : ";
			name.append(object->getTag());
			ImGui::Text(name.c_str());
			name = "METALLIC : ";
			float ambiant = object->getShader()->metallic;
			name.append(to_string(ambiant));
			ImGui::Text(name.c_str());
			name = "ROUGHNESS : ";
			float specular = object->getShader()->roughness;
			name.append(to_string(specular));
			ImGui::Text(name.c_str());
			name = "Use normal map : ";
			if (object->doesMeshHasNormalMap()) { name.append("Yes"); }
			else { name.append("No"); }
			ImGui::Text(name.c_str());
			name = "Use roughness map : ";
			if (object->doesMeshHasRoughnessMap()) { name.append("Yes"); }
			else { name.append("No"); }
			ImGui::Text(name.c_str());
			name = "Use metallic map : ";
			if (object->doesMeshHasMetallicMap()) { name.append("Yes"); }
			else { name.append("No"); }
			ImGui::Text(name.c_str());
			ImGui::Separator();

		}
		ImGui::End();
	}
};