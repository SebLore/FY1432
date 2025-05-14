#pragma once

#include <memory> // for std::unique_ptr
#include <imgui/imgui.h>

#include "Window.h"
#include "Shader.h"

class Application
{
public:
    Application(int windowWidth, int windowHeight, const char* windowTitle);
    ~Application();

    // Rule of five/three
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void Run();

private:
    void InitializeSubsystems(int windowWidth, int windowHeight, const char * windowName);
    void ShutdownSubsystems();
    bool glfwInitState();

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();

    void InitImGui();
    void ShutdownImGui();
    void BeginImGuiFrame();
    void RenderImGui();

    std::unique_ptr<Window> m_window;

	float m_dpiScale = 1.0f; // DPI scale for high-DPI displays

    // ImGui related state
    bool m_showDemoWindow = true;
	ImVec4 m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Clear color

    bool m_isRunning = false;

    // static callback function for when window size changes and dpi needs to be adjusted
    static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);


    // shader data (move to dedicated classes later)
    std::unique_ptr<Shader> m_ModelShader; // New shader object

    // For basic model data (will move to Model/Mesh classes later)
    unsigned int m_ModelVAO = 0;
    unsigned int m_ModelVBO = 0;
    unsigned int m_ModelEBO = 0;
    unsigned int m_ModelIndexCount = 0;
};