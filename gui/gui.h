#pragma once
#include "../display/display.h"
#include "../scene/scene.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using namespace std;
class gui
{
public:
	gui(string tag, display* display_);
	void setVisibility(bool isVisible_);
	virtual void update(scene* scene_) = 0;
	~gui();
protected:
	string m_tag;
	display* m_display;
	bool isVisible;

	void setupUIStyle();
};