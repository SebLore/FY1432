
// std libraries
#include <iostream>

// ImGui libraries
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


// OpenGL libraries
#include <glad/gl.h>
#include <GLFW/glfw3.h>


// stb libraries
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// Shader sources
const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)glsl";

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f); // orange color
    }
)glsl";


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

/*
* framebuffer_size_callback
* 
* This function is called whenever the window is resized.
* It sets the viewport to match the new window size.
* 
* @param window The GLFW window that was resized.
* @param width The new width of the window.
* @param height The new height of the window.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main(int, char**)
{
	// Initialize GLFW
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit()) {
		return -1;
	}

	// Configure GLFW (e.g., OpenGL version 3.3 Core Profile)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set OpenGL version to 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile, no legacy features

	// Create a window with graphics context
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Set the framebuffer size callback to resize the viewport

	// Initialize glad (load OpenGL function pointers)
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// --- Shader Compilation ---
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); // Delete shaders as they're linked into our program now and no longer necessary
	glDeleteShader(fragmentShader);

	// --- Vertex Data and Buffers ---
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left
		 0.5f, -0.5f, 0.0f, // right
		 0.0f,  0.5f, 0.0f  // top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VBO (optional, VAO still keeps track of it)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO (important to prevent accidental modifications)
	glBindVertexArray(0);

	// --- ImGui Setup ---
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	ImGui::StyleColorsDark(); // Setup style

	// backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");


	// states
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);




	// Main render loop
	while (!glfwWindowShouldClose(window)) {
		// Input
		processInput(window);
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
			ImGui_ImplGlfw_Sleep(10);
			continue;
		} // Sleep if the window is minimized

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;
			ImGui::Begin("Hello, world!"); // Create a window
			ImGui::Text("This is some useful text."); // Display some text
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering
		ImGui::Render();

		// Render
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w); // Dark cyan background
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw the triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // Seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // No need to unbind it every frame

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// GLFW: swap buffers and poll IO events
		glfwSwapBuffers(window);
	}

	// Cleanup
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}