#include "Window.h"

#include <glad/gl.h>    
#include <GLFW/glfw3.h> 

#include <stdexcept>
#include <iostream>

void Window::GlfwErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

void Window::FramebufferSizeCallback(GLFWwindow* glfwWindow, int width, int height) {
	Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
	if (windowInstance) {
		windowInstance->m_width = width;
		windowInstance->m_height = height;
		glViewport(0, 0, width, height);
		// You could add a user-defined resize callback here if needed
	}
}

Window::Window(int width, int height, const char* title)
	: m_width(width), m_height(height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	m_nativeWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (!m_nativeWindow) {
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwSetWindowUserPointer(m_nativeWindow, this); // Link this instance to the GLFW window
	glfwSetFramebufferSizeCallback(m_nativeWindow, FramebufferSizeCallback);

	MakeContextCurrent();
	glfwSwapInterval(1); // Enable V-Sync
}

Window::~Window() {
	std::cout << "Destroying window (" << m_width << ", " << m_height << ")..." << std::endl;

	if (m_nativeWindow) {
		glfwDestroyWindow(m_nativeWindow);
		std::cout << "Window::GLFW window destroyed." << std::endl;
	}
	m_nativeWindow = nullptr;
	std::cout << "Window destroyed." << std::endl;
}

void Window::MakeContextCurrent() const {
	glfwMakeContextCurrent(m_nativeWindow);
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(m_nativeWindow);
}

void Window::SwapBuffers() const {
	glfwSwapBuffers(m_nativeWindow);
}