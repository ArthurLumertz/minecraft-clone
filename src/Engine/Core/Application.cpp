#include "Application.h"
#include <GLFW/glfw3.h>
#include "Input.h"

Application::Application(int width, int height, const char* title)
    : m_window(width, height, title) {

    m_window.setResizeCallback(
        [this](int width, int height) {
            onResize(width, height);
        }
    );
}

void Application::run() {
    float lastFrame = glfwGetTime();
    float lastTimer = glfwGetTime();
    int frames = 0;

    while (m_window.isOpen()) {
        float now = glfwGetTime();
        float deltaTime = now - lastFrame;
        lastFrame = now;

        if (now - lastTimer > 1) {
            std::printf("FPS: %d\n", frames);
            frames = 0;
            lastTimer = now;
        }

        m_window.pollEvents();

        update(deltaTime);

        render();
        frames++;

        m_window.swapBuffers();
        
        Input::endFrame();
    }
}

void Application::framebufferSizeCallback(
    GLFWwindow* window,
    int width,
    int height
) {
    glViewport(0, 0, width, height);

    Application* app =
        static_cast<Application*>(
            glfwGetWindowUserPointer(window)
        );

    if (app) {
        app->onResize(width, height);
    }
}