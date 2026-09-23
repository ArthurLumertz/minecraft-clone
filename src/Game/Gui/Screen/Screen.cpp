#include "Screen.h"
#include "Widget/Widget.h"

Screen::~Screen() = default;

void Screen::init() {
}

void Screen::update(float delta) {
    for (auto& widget : m_widgets) {
        widget->update(delta);
    }
}

void Screen::render(Renderer& renderer, Camera& camera) {
    for (auto& widget : m_widgets) {
        widget->render(renderer, camera);
    }
}

void Screen::onClose() {
}