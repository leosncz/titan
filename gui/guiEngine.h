#pragma once
#include "gui.h"
class guiEngine : public gui
{
public:
	guiEngine(display* display_) : gui(display_)
	{
		m_showLightingDebug = false;
		m_showRenderingDebug = false;
		m_showSceneInformations = false;
		m_showHelloMessage = true;
	}

	void render(scene* scene_)
	{
		if (glfwGetKey(m_display->getGLFWWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			isVisible = !isVisible;
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
				showRenderingDebug(scene_->getGPosition(), scene_->getGNormals(), scene_->getGAlbedo(), scene_->getGRoughness(), scene_->getGMetallic(),scene_->getGAmbient());
			}
			if (m_showSceneInformations)
			{
				showSceneInformations(scene_->getObjectHolder());
			}
			if (m_showLightingDebug)
			{
				showLightingDebug(*scene_->getLights());
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
				if (ImGui::BeginMenu("Scene editor"))
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

	void showRenderingDebug(GLuint positionTexture, GLuint normalTexture, GLuint albedoSpecTexture, GLuint roughnessTexture, GLuint metallicTexture, GLuint ambientTexture)
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

		ImGui::Separator();

		ImGui::TextWrapped("Ambient: ");
		IM_ASSERT(ambientTexture);
		ImGui::Image((void*)(intptr_t)ambientTexture, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));

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
