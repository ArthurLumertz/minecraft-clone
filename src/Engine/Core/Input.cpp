#include "Input.h"

bool Input::m_keysDown[GLFW_KEY_LAST + 1] = { false };
bool Input::m_keysReleased[GLFW_KEY_LAST + 1] = { false };
bool Input::m_buttonsDown[GLFW_MOUSE_BUTTON_LAST + 1] = { false };
bool Input::m_buttonsReleased[GLFW_MOUSE_BUTTON_LAST + 1] = { false };

glm::vec2 Input::m_oldMousePosition = { 0.0f, 0.0f };
glm::vec2 Input::m_mousePosition = { 0.0f, 0.0f };
glm::vec2 Input::m_mouseDelta = { 0.0f, 0.0f };

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key < 0 || key >= GLFW_KEY_LAST) {
		return;
	}
	m_keysDown[key] = action != GLFW_RELEASE;
	m_keysReleased[key] = action == GLFW_PRESS;
}

void Input::cursorPositionCallback(
	GLFWwindow* window,
	double xpos,
	double ypos
) {
	glm::vec2 newPosition(
		static_cast<float>(xpos),
		static_cast<float>(ypos)
	);

	m_mouseDelta += newPosition - m_mousePosition;
	m_mousePosition = newPosition;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button < 0 || button >= GLFW_MOUSE_BUTTON_LAST) {
		return;
	}
	m_buttonsDown[button] = action != GLFW_RELEASE;
	m_buttonsReleased[button] = action == GLFW_PRESS;
}

void Input::endFrame() {
	m_mouseDelta = glm::vec2(0.0f);

	std::fill(
		std::begin(m_keysReleased),
		std::end(m_keysReleased),
		false
	);

	std::fill(
		std::begin(m_buttonsReleased),
		std::end(m_buttonsReleased),
		false
	);
}