#pragma once

// Forward declare to avoid including glfw3.h in this public header
struct GLFWwindow;

class Window
{
public:
    Window(int width, int height, const char* title);
    ~Window();

    // Rule of five/three - for now, make non-copyable and non-movable
    // to simplify resource management with the raw GLFWwindow pointer.
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(Window&&) = delete;

    bool ShouldClose() const;
    void SwapBuffers() const;
    void MakeContextCurrent() const;

    GLFWwindow* GetNativeHandle() const { return m_nativeWindow; }
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    // Static callback functions (can be expanded)
    static void GlfwErrorCallback(int error, const char* description);
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

private:
    GLFWwindow* m_nativeWindow = nullptr;
    int m_width;
    int m_height;
};