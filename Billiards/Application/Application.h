#pragma once

#include <memory> // for std::unique_ptr
#include <imgui.h>

#include "Window.h"
#include "Shader.h"
#include "Camera.h"
#include "ModelLoader.h"

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

    void InitializeShaders();
    void InitResources(int width, int height);

    void ProcessInput(float deltaTime);
    void Update(float deltaTime);
    void Render();

    void InitImGui();
    void ShutdownImGui();
    void BeginImGuiFrame();
    void RenderImGui();

    // --- member variables -----------------------------------------------------------------------
    std::unique_ptr<Window> m_window;
    bool m_isRunning = false;
    
    // for imgui
    bool m_showDemoWindow = true;
	ImVec4 m_clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // Clear color


    // static callback function for when window size changes and dpi needs to be adjusted
	float m_dpiScale = 1.0f; // DPI scale for high-DPI displays
    static void WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale);


	std::unique_ptr<Camera> m_camera; // Camera object TODO: move into some kind of scene class
	std::unique_ptr<Shader> m_modelShader; // New shader object TODO: move to dedicated shader manager
	std::unique_ptr<Model> m_model; // Model object TODO: move to some kind of scene class
};