#pragma once

#include "Window.h"

class Application {
public:
	Application(int width, int height, const char* title);

	void run();

	static void framebufferSizeCallback(
		GLFWwindow* window,
		int width,
		int height
	);

protected:
	virtual void update(float deltaTime) = 0;
	virtual void render() = 0;
	virtual void onResize(int width, int height) = 0;

	Window m_window;
};

