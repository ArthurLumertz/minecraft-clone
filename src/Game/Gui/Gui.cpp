#include "Gui.h"
#include "Screen/Screen.h"
#include "Hud/Hud.h"

Gui::Gui(int width, int height):
    m_width(width),
    m_height(height),
    m_atlas("resources/gui.png", true),
    m_hud(std::make_unique<Hud>(width, height, m_atlas)) {

}

Gui::~Gui() = default;

void Gui::update(float delta) {
    m_hud->update(delta);

    if (m_screen) {
        m_screen->update(delta);
    }
}

void Gui::render(Renderer& renderer, Camera& camera) {
    m_hud->render(renderer, camera);

    if (m_screen) {
        m_screen->render(renderer, camera);
    }
}

void Gui::setScreen(std::unique_ptr<Screen> screen) {
    m_screen = std::move(screen);

    if (m_screen) {
        m_screen->init();
    }
}

void Gui::closeScreen() {
    m_screen.reset();
}