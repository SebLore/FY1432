#pragma once

#include <imgui/imgui.h>
#include <GLFW/glfw3.h>

class ImGuiWrapper
{
public:
	ImGuiWrapper(GLFWwindow * window, float dpiScale);
	~ImGuiWrapper() = default;

	void init();
	void newFrame();
	void endFrame();
	void render();
	void shutdown();
private:
	GLFWwindow* window;
	float m_dpiScale;
};
