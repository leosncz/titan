#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../display/display.h"
#include <iostream>
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
	}
	void setVisibility(bool isVisible_) { isVisible = isVisible_; }
	void update()
	{
		if(isVisible)
		{
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Another Window", &isVisible);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				isVisible = false;
			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
	}
private:
	int m_id;
	display* m_display;
	bool isVisible;
};