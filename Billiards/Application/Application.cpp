#include "Application.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/ext/matrix_clip_space.hpp>

#include "Window.h" // Needs full Window definition

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <stdexcept>
#include <chrono> // For delta time


// static callback for dynamic window scale
void Application::WindowContentScaleCallback(GLFWwindow* window, float xscale, float yscale)
{
	// This callback is optional and can be used to adjust the DPI scale dynamically
	Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
	// Check if the application instance is valid
	if (app)
	{
		app->m_dpiScale = (xscale + yscale) * 0.5f; // Average scale

		ImGuiIO& io = ImGui::GetIO();
		io.FontGlobalScale = app->m_dpiScale; // Update ImGui font scale
	}
}

Application::Application(int windowWidth, int windowHeight, const char* windowTitle)
	: m_dpiScale(1.0f), m_showDemoWindow(false), m_isRunning(false)
{
	try
	{
		InitializeSubsystems(windowWidth, windowHeight, windowTitle);
		m_isRunning = true;
	}
	catch (const std::exception& e) {
		std::cerr << "Application Initialization Error: " << e.what() << std::endl;
		ShutdownSubsystems(); // Clean up already initialized parts
		throw; // Re-throw to be caught by main
	}
}

Application::~Application()
{
	std::cout << "Shutting down application." << std::endl;
	ShutdownSubsystems();

	std::cout << "Application destroyed." << std::endl;
}

void Application::InitializeSubsystems(int windowWidth, int windowHeight, const char* windowTitle)
{
	try
	{
		std::cout << "Initializing Application subsystems..." << std::endl;

		// 1. Initialize GLFW
		glfwSetErrorCallback(Window::GlfwErrorCallback);
		if (!glfwInit()) 
		{
			throw std::runtime_error("Failed to initialize GLFW");
		}
		std::cout << "GLFW initialized successfully." << std::endl;

		// 2. Get DPI so window and ImGui scale is correct across different resolutions
		GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
		if (primaryMonitor)
		{
			float xscale, yscale;
			glfwGetMonitorContentScale(primaryMonitor, &xscale, &yscale);
			m_dpiScale = (xscale + yscale) / 2.0f; // take average
			std::cout << "DPI Scale: " << m_dpiScale << std::endl;
		}
		else
		{
			std::cerr << "Warning: Failed to get primary monitor for DPI scale." << std::endl;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		int width = static_cast<int>(windowWidth * m_dpiScale);
		int height = static_cast<int>(windowHeight * m_dpiScale);

		m_window = std::make_unique<Window>(width, height, windowTitle);

		// link the application's window instance to the GLFW window
		glfwSetWindowUserPointer(m_window->GetNativeHandle(), this);
		glfwSetWindowContentScaleCallback(m_window->GetNativeHandle(), WindowContentScaleCallback);


		// 2. Initialize OpenGL using GLAD
		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
		{
			throw std::runtime_error("Failed to initialize GLAD");
		}
		std::cout << "GLAD initialized successfully." << std::endl;
		glEnable(GL_DEPTH_TEST); // enable depth testing for 3D rendering

		// 3. Initialize ImGui
		InitImGui();

		// 4. Initialize Shaders
		InitializeShaders();

		// 5. Initialize resources
		InitResources(windowWidth, windowHeight);

		std::cout << "Subsystems initialized." << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Window Initialization Error: " << e.what() << std::endl;
		if (glfwInitState())
			glfwTerminate(); // clean up GLFW if it was initialized
		throw; // pass the exception up to main
	}

}

void Application::ShutdownSubsystems() 
{
	if (m_isRunning || m_window) { // make sure shutdown happens if instance was created
		std::cout << "Shutting down application subsystems..." << std::endl;
		ShutdownImGui();
	}

	m_modelShader.reset();

	// m_window will destruct itself and terminate GLFW
}

bool Application::glfwInitState()
{
	return glfwGetPrimaryMonitor() != nullptr;
}

void Application::InitializeShaders()
{
	try 
	{
		m_modelShader = std::make_unique<Shader>("shaders/model.vert", "shaders/model.frag");
		std::cout << "Model shader loaded successfully." << std::endl;
	}
	catch (const std::exception& e) 
	{
		std::cerr << "Failed to load model shader: " << e.what() << std::endl;
		throw;
	}
}

void Application::InitResources(int width, int height)
{
	try
	{
		m_camera = std::make_unique<Camera>(static_cast<float>(width), static_cast<float>(height), 0.1f, 100.0f);
		m_model = ModelLoader::LoadModel("assets/geometry/teapot.obj");
		if (!m_model)
		{
			throw std::runtime_error("Failed to load model.");
		}
		std::cout << "Model loaded successfully." << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load model: " << e.what() << std::endl;
	}
}


void Application::InitImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Optional
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos; // Optional

	ImGui::StyleColorsDark();
	// ImGui::StyleColorsClassic();

	ImGuiStyle& style = ImGui::GetStyle(); //Optional style editing
	style.ScaleAllSizes(m_dpiScale); // scale GUI for different monitor resolution
	io.FontGlobalScale = m_dpiScale; // scale font size for different monitor resolution


	ImGui_ImplGlfw_InitForOpenGL(m_window->GetNativeHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
	std::cout << "ImGui initialized." << std::endl;
}

void Application::ShutdownImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Application::BeginImGuiFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Application::RenderImGui() {
	// --- Render ImGui UI ---
	BeginImGuiFrame();

	if (m_showDemoWindow) {
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}
	ImGui::Begin("My Application Controls");
	ImGui::Text("Hello from Application class!");
	ImGui::Checkbox("Show ImGui Demo Window", &m_showDemoWindow);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// TODO: Handle ImGui input events
	/*ImGuiIO& io = ImGui::GetIO();*/
}

void Application::ProcessInput(float deltaTime) {
	if (glfwGetKey(m_window->GetNativeHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window->GetNativeHandle(), true);
	}

	// TODO: implement camera controls
	// W to move camera forward
	if (glfwGetKey(m_window->GetNativeHandle(), GLFW_KEY_W) == GLFW_PRESS) {
		m_camera->MoveForward(deltaTime);
	}
	// S to move camera backward
	if (glfwGetKey(m_window->GetNativeHandle(), GLFW_KEY_S) == GLFW_PRESS) {
		m_camera->MoveForward(-deltaTime);
	}
}

void Application::Update(float deltaTime)
{
	// TODO: add model update loop
}

void Application::Render() {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Add GL_DEPTH_BUFFER_BIT if doing 3D

	// --- Apply shader and set uniforms ---
	m_modelShader->Use();

	m_modelShader->SetMat4("uViewProj", m_camera->GetViewProjectionMatrix());
	m_modelShader->SetMat4("uModel", m_model->GetTransform());
	m_modelShader->SetVec3("uViewPos", m_camera->GetPosition());

	// --- Do draw calls here
	m_model->Draw(); // Draw the model


	// --- Render ImGui UI ---
	RenderImGui(); // this handles ImGui::Render() and drawing the data, also ends the ImGui frame
}

void Application::Run() {
	if (!m_window) {
		std::cerr << "Window not initialized in Application. Cannot run." << std::endl;
		return;
	}
	m_isRunning = true; // Set running state after all critical inits

	auto lastTime = std::chrono::high_resolution_clock::now();


	// main window loop
	while (m_isRunning && !m_window->ShouldClose())
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
		lastTime = currentTime;

		glfwPollEvents(); // Poll events at the start of the frame

		// application bits
		{
			ProcessInput(deltaTime);
			Update(deltaTime);
			Render();
		}

		m_window->SwapBuffers();
	}
}