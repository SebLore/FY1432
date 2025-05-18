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
		if (!glfwInit()) {
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
	
		// --- Temporary OpenGL Object Creation (Remove the old m_ShaderProgram, m_Vao, m_Vbo setup for the triangle) ---
		InitializeModel();
		// --- End of temporary OpenGL object creation ---

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

void Application::ShutdownSubsystems() {
	if (m_isRunning || m_window) { // make sure shutdown happens if instance was created
		std::cout << "Shutting down application subsystems..." << std::endl;
		ShutdownImGui();
	}

	m_ModelShader.reset();

	 // --- Cleanup OpenGL Objects (Move to class destructors later)  ---
	// glDeleteProgram(m_ShaderProgram); // Old one, remove
	if (m_ModelVAO != 0) glDeleteVertexArrays(1, &m_ModelVAO);
	if (m_ModelVBO != 0) glDeleteBuffers(1, &m_ModelVBO);
	if (m_ModelEBO != 0) glDeleteBuffers(1, &m_ModelEBO); // If you add EBO
	// --- End of OpenGL object cleanup ---

	// m_window will destruct itself and terminate GLFW

}

bool Application::glfwInitState()
{
	return glfwGetPrimaryMonitor() != nullptr;
}

void Application::InitializeShaders()
{
	try {
		m_ModelShader = std::make_unique<Shader>("shaders/model.vert", "shaders/model.frag");
		std::cout << "Model shader loaded successfully." << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to load model shader: " << e.what() << std::endl;
		// Handle error, perhaps rethrow or set a bad state
	}
}

void Application::InitializeModel()
{
	float vertices[] = {
		// positions         // normals (dummy for now) // texCoords (dummy)
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 1.0f
	};
	glGenVertexArrays(1, &m_ModelVAO);
	glGenBuffers(1, &m_ModelVBO);
	glBindVertexArray(m_ModelVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ModelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	m_ModelIndexCount = 3; // Not using EBO for this simple triangle array draw
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
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// TODO: Handle ImGui input events
	/*ImGuiIO& io = ImGui::GetIO();*/
}



void Application::ProcessInput(float deltaTime) {
	if (glfwGetKey(m_window->GetNativeHandle(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(m_window->GetNativeHandle(), true);
	}
	// Add other input processing here (e.g., for camera, player, etc.)
}

void Application::Update(float deltaTime) {
	// Update simulation logic, physics, etc.
}

void Application::Render() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Add GL_DEPTH_BUFFER_BIT if doing 3D

	// --- Your OpenGL scene rendering would go here ---
	// For now, we're just clearing the screen.
	// The triangle/model drawing will be added back here later.


	if(m_ModelShader && m_ModelVAO != 0) {
		m_ModelShader->Use();

		glm::mat4 projection = glm::perspective(glm::radians(m_camera->GetZoom()),
			(float)m_window->GetWidth() / (float)m_window->GetHeight(),
			0.1f, 100.0f);
		glm::mat4 view = m_camera->GetViewMatrix();
		m_ModelShader->SetMat4("projection", projection);
		m_ModelShader->SetMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		// model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f)); // Optional: test rotation
		m_ModelShader->SetMat4("model", model);

		// Set lighting/material uniforms for the new shader
		m_ModelShader->SetVec3("objectColor", 1.0f, 0.5f, 0.31f); // e.g., coral
		m_ModelShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		m_ModelShader->SetVec3("lightPos_World", 1.2f, 1.0f, 2.0f);
		m_ModelShader->SetVec3("viewPos_World", m_camera->GetPosition());

		glBindVertexArray(m_ModelVAO);
		// If using EBO: glDrawElements(GL_TRIANGLES, m_ModelIndexCount, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3); // For the simple triangle VBO
		glBindVertexArray(0);
	}


	// --- Render ImGui UI ---
	BeginImGuiFrame();

	if (m_showDemoWindow) {
		ImGui::ShowDemoWindow(&m_showDemoWindow);
	}
	ImGui::Begin("My Application Controls");
	ImGui::Text("Hello from Application class!");
	ImGui::Checkbox("Show ImGui Demo Window", &m_showDemoWindow);
	ImGui::End();

	RenderImGui(); // This handles ImGui::Render() and drawing the data
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
			std::cout << "Pretending to render...\n";
			//ProcessInput(deltaTime);
			//Update(deltaTime);
			//Render();
		}

		m_window->SwapBuffers();
	}
}