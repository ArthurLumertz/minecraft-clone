#pragma once

#include "../../Engine/Graphics/Texture/Texture.h"
#include <memory>

class Screen;
class Hud;
class Renderer;
class Camera;
class Texture;

class Gui {
public:
	Gui(int width, int height);
	~Gui();

	void update(float delta);
	void render(Renderer& renderer, Camera& camera);

	void setScreen(std::unique_ptr<Screen> screen);
	void closeScreen();

	Screen* getScreen() { return m_screen.get(); }
	bool hasScreen() const { return m_screen != nullptr; }

	Texture& getAtlas() { return m_atlas; }

private:
	const int m_width;
	const int m_height;

	std::unique_ptr<Screen> m_screen;
	std::unique_ptr<Hud> m_hud;

	Texture m_atlas;
};