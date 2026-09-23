#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>

class Texture;

class Window {
public:
    using ResizeCallback = std::function<void(int, int)>;

    Window(int width, int height, const std::string& title);
    ~Window();

    bool isOpen();

    void swapBuffers();
    void pollEvents();

    void toggleFullscreen();
    void setIcon(const Texture& texture);

    void setResizeCallback(ResizeCallback callback) {
        m_resizeCallback = std::move(callback);
    }

private:
    static void framebufferSizeCallback(
        GLFWwindow* window,
        int width,
        int height
    );

private:
    GLFWwindow* m_window = nullptr;

    int m_width;
    int m_height;

    bool m_fullscreen = false;

    int m_windowedX = 0;
    int m_windowedY = 0;
    int m_windowedWidth = 0;
    int m_windowedHeight = 0;

    ResizeCallback m_resizeCallback;
};