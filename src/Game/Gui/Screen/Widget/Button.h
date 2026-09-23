#pragma once

#include "Widget.h"

#include <string>
#include <functional>

class Button : public Widget {
public:
    Button(
        float x,
        float y,
        float width,
        float height,
        const std::string& text,
        std::function<void()> onPress
    );

    void update(float delta) override;
    void render(Renderer& renderer, Camera& camera) override;

private:
    std::string m_text;

    std::function<void()> m_onPress;
};