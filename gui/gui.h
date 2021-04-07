#pragma once
#include <GL/glew.h>
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
		m_showLightingDebug = false;
		setupUIStyle();
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
	int m_id;
	display* m_display;
	bool isVisible;

	bool m_showRenderingDebug, m_showSceneInformations, m_showLightingDebug;

	void showLightingDebug(vector<light*> lights)
	{
		ImGui::Begin("Lighting & shadow debug", &isVisible);
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
		ImGui::Begin("Deferred rendering debug", &isVisible);
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
		ImGui::Begin("Hello", &isVisible);
		ImGui::SetWindowSize(ImVec2(600, 200));
		ImGui::SetWindowFontScale(1.1);
		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Welcome to TITAN Engine !");
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

	void setupUIStyle()
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		io.Fonts->AddFontFromFileTTF("graphicData/OpenSans-Light.ttf", 20);

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f); 
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	}
};