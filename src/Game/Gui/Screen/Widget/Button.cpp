#include "Button.h"
#include "../../../../Engine/Core/Input.h"

Button::Button(
    float x,
    float y,
    float width,
    float height,
    const std::string& text,
    std::function<void()> onPress
)
    : Widget(x, y, width, height),
    m_text(text),
    m_onPress(std::move(onPress)) {
}

void Button::update(float delta) {
    glm::vec2 mouse = Input::getMousePosition();

    m_hovered = isMouseOver(mouse.x, mouse.y);

    if (
        m_hovered &&
        Input::isMouseButtonDown(MouseButton::LEFT)
    ) {
        if (m_onPress) {
            m_onPress();
        }
    }
}

void Button::render(Renderer& renderer, Camera& camera) {
    // Later:
    //
    // renderer.drawQuad(...)
    // renderer.drawString(...)
}