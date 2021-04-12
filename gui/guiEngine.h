#pragma once
#include "gui.h"
class guiEngine : public gui
{
public:
	guiEngine(display* display_) : gui(display_)
	{
		m_showLightingEditor = false;
		m_showRenderingDebug = false;
		m_showSceneEditor = false;
		m_showHelloMessage = true;
	}

	void update(scene* scene_)
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
			if (m_showSceneEditor)
			{
				showSceneEditor(scene_->getObjectHolder());
			}
			if (m_showLightingEditor)
			{
				showLightingEditor(scene_->getLights());
			}

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Rendering debug"))
				{
					m_showRenderingDebug = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Lighting editor"))
				{
					m_showLightingEditor = true;
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Scene editor"))
				{
					m_showSceneEditor = true;
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
	bool m_showRenderingDebug, m_showSceneEditor, m_showLightingEditor, m_showHelloMessage;

	void showLightingEditor(vector<light*>* lights)
	{
		ImGui::Begin("Lighting editor", &m_showLightingEditor);
		ImGui::SetWindowFontScale(1.1);
		if (lights->size() == 0)
		{
			ImGui::TextWrapped("No light !");
		}
		else
		{
			for (int i = 0; i < lights->size(); i++)
			{
				IM_ASSERT(lights->at(i)->textureDepthMap);
				string content = "Light ";
				content.append(std::to_string(i));
				content.append(" :");
				ImGui::TextWrapped(content.c_str());
				content = "Shadow resolution: ";
				content.append(to_string((lights->at(i)->shadowResolution)));
				ImGui::TextWrapped(content.c_str());
				if (lights->at(i)->type == DIRECTIONNAL_LIGHT)
				{
					ImGui::TextWrapped("Type: Directionnal");
					content = "ShadowMap (if enabled): ";
					ImGui::TextWrapped(content.c_str());
					ImGui::Image((void*)(intptr_t)lights->at(i)->textureDepthMap, ImVec2(400, 300), ImVec2(0, 1), ImVec2(1, 0));
				}
				else
				{
					ImGui::TextWrapped("Type: Point light");
				}

				content = "constant: ";
				content.append(to_string(lights->at(i)->constant));
				ImGui::TextWrapped(content.c_str());
				content = "linear: ";
				content.append(to_string(lights->at(i)->linear));
				ImGui::TextWrapped(content.c_str());
				content = "quadratic: ";
				content.append(to_string(lights->at(i)->quadratic));
				ImGui::TextWrapped(content.c_str());

				content = "Delete ";
				content.append(to_string(i));
				if (ImGui::Button(content.c_str()))
				{
					lights->erase(lights->begin() + i);
				}

				ImGui::Separator();
			}
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

	void showSceneEditor(vector<renderObject*>* objectHolder)
	{
		//Scene information
		ImGui::Begin("Scene editor", &m_showSceneEditor);
		ImGui::SetWindowFontScale(1.1);
		for (int i = 0; i < objectHolder->size(); i++)
		{
			vector<renderObject*>* test = objectHolder;
			renderObject* object = test->at(i);
			string name = "ID : ";
			name.append(to_string(object->getID()));
			ImGui::Text(name.c_str());

			name = "Tag : ";
			name.append(object->getTag());
			ImGui::Text(name.c_str());

			name = "Metallic : ";
			float metallic = object->getShader()->metallic;
			name.append(to_string(metallic));
			ImGui::Text(name.c_str());

			name = "Roughness : ";
			float roughness = object->getShader()->roughness;
			name.append(to_string(roughness));
			ImGui::Text(name.c_str());

			name = "Ambient : ";
			float ambient = object->getShader()->ambient;
			name.append(to_string(ambient));
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