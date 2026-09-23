#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class Key {
	W = GLFW_KEY_W,
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,

	SPACE = GLFW_KEY_SPACE,

	LSHIFT = GLFW_KEY_LEFT_SHIFT,
	LCONTROL = GLFW_KEY_LEFT_CONTROL,

	ESCAPE = GLFW_KEY_ESCAPE,
	F11 = GLFW_KEY_F11
};

enum class MouseButton {
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
	MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE,
};

class Input {
private:
	static bool m_keysDown[GLFW_KEY_LAST + 1];
	static bool m_keysReleased[GLFW_KEY_LAST + 1];
	static bool m_buttonsDown[GLFW_MOUSE_BUTTON_LAST + 1];
	static bool m_buttonsReleased[GLFW_MOUSE_BUTTON_LAST + 1];

	static glm::vec2 m_mousePosition;
	static glm::vec2 m_oldMousePosition;
	static glm::vec2 m_mouseDelta;

public:
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static bool isKeyDown(Key key) { return m_keysDown[static_cast<int>(key)]; }
	static bool isKeyReleased(Key key) { bool temp = m_keysReleased[static_cast<int>(key)]; m_keysReleased[static_cast<int>(key)] = false; return temp; }

	static bool isMouseButtonDown(MouseButton button) { return m_buttonsDown[static_cast<int>(button)]; }
	static bool isMouseButtonReleased(MouseButton button) { bool temp = m_buttonsReleased[static_cast<int>(button)]; m_buttonsReleased[static_cast<int>(button)] = false; return temp; }

	static glm::vec2 getMousePosition() { return m_mousePosition; }
	static glm::vec2 getMouseDelta() { return m_mouseDelta; }

	static void endFrame();


};
