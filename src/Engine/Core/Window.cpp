#include "Window.h"
#include "Input.h"
#include <stdexcept>
#include "../Graphics/Texture/Texture.h"

Window::Window(int width, int height, const std::string& title) : m_width(width), m_height(height) {
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize GLFW!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!m_window) {
		throw std::runtime_error("Failed to create GLFW window!");
	}

	glfwSetKeyCallback(m_window, Input::keyCallback);
	glfwSetMouseButtonCallback(m_window, Input::mouseButtonCallback);
	glfwSetCursorPosCallback(m_window, Input::cursorPositionCallback);
    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(
        m_window,
        framebufferSizeCallback
    );

	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to load GLAD!");
	}

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

    setIcon(Texture("resources/icon.png", false));

	glfwSwapInterval(0);

	glfwShowWindow(m_window);
}

Window::~Window() {
	glfwTerminate();
	glfwDestroyWindow(m_window);
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::toggleFullscreen() {
    m_fullscreen = !m_fullscreen;

    if (m_fullscreen) {
        glfwGetWindowPos(
            m_window,
            &m_windowedX,
            &m_windowedY
        );

        glfwGetWindowSize(
            m_window,
            &m_windowedWidth,
            &m_windowedHeight
        );

        GLFWmonitor* monitor =
            glfwGetPrimaryMonitor();

        const GLFWvidmode* mode =
            glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(
            m_window,
            monitor,
            0,
            0,
            mode->width,
            mode->height,
            mode->refreshRate
        );
    }
    else {
        glfwSetWindowMonitor(
            m_window,
            nullptr,
            m_windowedX,
            m_windowedY,
            m_windowedWidth,
            m_windowedHeight,
            GLFW_DONT_CARE
        );
    }
}

void Window::framebufferSizeCallback(
    GLFWwindow* window,
    int width,
    int height
) {
    Window* self = static_cast<Window*>(
        glfwGetWindowUserPointer(window)
    );

    if (!self) {
        return;
    }

    self->m_width = width;
    self->m_height = height;

    glViewport(0, 0, width, height);

    if (self->m_resizeCallback) {
        self->m_resizeCallback(width, height);
    }
}

void Window::setIcon(const Texture& texture) {
    GLFWimage icon;
    icon.width = texture.getWidth();
    icon.height = texture.getHeight();
    icon.pixels = texture.getRawPixels();

    glfwSetWindowIcon(
        m_window,
        1,
        &icon
    );
}