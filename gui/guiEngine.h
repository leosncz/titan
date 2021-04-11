#pragma once
#include "gui.h"
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../display/display.h"
#include "../renderObject/renderObject.h"
#include "../camera/camera.h"
#include <iostream>
#include <vector>
class guiEngine : public gui
{
public:
	guiEngine(camera* cam)
	{
		m_showLightingDebug = false;
		m_showRenderingDebug = false;
		m_showSceneInformations = false;
		m_showHelloMessage = true;
		m_camera = cam;
	}
	void update(vector<renderObject*>* objectHolder, vector<light*> lights, GLuint albedoSpecTexture, GLuint normalTexture, GLuint positionTexture, GLuint roughnessTexture, GLuint metallicTexture)
	{
		if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			isVisible = !isVisible;
			if (isVisible == true)
			{
				m_camera->pauseControls();
			}
			else
			{
				m_camera->resumeControls();
			}
		}
		if (isVisible)
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (m_showHelloMessage)
			{
				showWelcome();
			}
			if (m_showRenderingDebug)
			{
				showRenderingDebug(positionTexture, normalTexture, albedoSpecTexture, roughnessTexture, metallicTexture);
			}
			if (m_showSceneInformations)
			{
				showSceneInformations(objectHolder);
			}
			if (m_showLightingDebug)
			{
				showLightingDebug(lights);
			}

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Rendering debug"))
				{
					m_showRenderingDebug = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Lighting & shadow debug"))
				{
					m_showLightingDebug = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene informations"))
				{
					m_showSceneInformations = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Quit"))
				{
					m_display->setExitStatus(true);
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
private:
	bool m_showRenderingDebug, m_showSceneInformations, m_showLightingDebug, m_showHelloMessage;
	camera* m_camera;

	void showLightingDebug(vector<light*> lights)
	{
		ImGui::Begin("Lighting & shadow debug", &m_showLightingDebug);
		ImGui::SetWindowFontScale(1.1);
		for (int i = 0; i < lights.size(); i++)
		{
			IM_ASSERT(lights[i]->textureDepthMap);
			string content = "Light " + i;
			ImGui::TextWrapped(content.c_str());
			content = "ShadowMap (if enabled): ";
			ImGui::TextWrapped(content.c_str());
			ImGui::Image((void*)(intptr_t)lights[i]->textureDepthMap, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

			ImGui::Separator();
		}
		ImGui::End();
	}

	void showRenderingDebug(GLuint positionTexture, GLuint normalTexture, GLuint albedoSpecTexture, GLuint roughnessTexture, GLuint metallicTexture)
	{
		ImGui::Begin("Deferred rendering debug", &m_showRenderingDebug);
		ImGui::SetWindowFontScale(1.1);

		char* renderer = (char*)glGetString(GL_RENDERER);
		char* vendor = (char*)glGetString(GL_VENDOR);
		ImGui::TextWrapped(renderer);
		ImGui::TextWrapped(vendor);
		ImGui::Separator();
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
		ImGui::Begin("Hello", &m_showHelloMessage);
		ImGui::SetWindowSize(ImVec2(600, 200));
		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welcome to TITAN Engine !");
		ImGui::TextWrapped("Keep in mind : This engine is still in early development !");
		ImGui::TextWrapped("The 'scene informations' panel only prints active objects");
		ImGui::TextWrapped("Press ESCAPE key to show/hide this interface");
		ImGui::TextColored(ImVec4(1, 0, 0, 1), "From github.com/leosncz/titan");
		ImGui::End();
	}

	void showSceneInformations(vector<renderObject*>* objectHolder)
	{
		//Scene information
		ImGui::Begin("Scene informations", &m_showSceneInformations);
		ImGui::SetWindowFontScale(1.1);
		float value;
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

			string buttonName = "Delete ";
			buttonName.append(to_string(object->getID()));
			if (ImGui::Button(buttonName.c_str())) { objectHolder->erase(objectHolder->begin() + i); }

			ImGui::Separator();

		}
		ImGui::End();
	}
};
