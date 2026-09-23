#pragma once

class Renderer;
class Camera;

class Widget {
public:
    Widget(float x, float y, float width, float height)
        : m_x(x),
        m_y(y),
        m_width(width),
        m_height(height) {
    }

    virtual ~Widget() = default;

    virtual void update(float delta) {}

    virtual void render(Renderer& renderer, Camera& camera) {}

    bool isMouseOver(float mouseX, float mouseY) const {
        return
            mouseX >= m_x &&
            mouseX <= m_x + m_width &&
            mouseY >= m_y &&
            mouseY <= m_y + m_height;
    }

protected:
    float m_x;
    float m_y;

    float m_width;
    float m_height;

    bool m_hovered = false;
    bool m_active = true;
};