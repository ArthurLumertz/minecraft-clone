#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "Widget/Widget.h"

class Renderer;
class Camera;

class Screen {
public:
    virtual ~Screen();

    virtual void init();

    virtual void update(float delta);
    virtual void render(Renderer& renderer, Camera& camera);

    virtual void onClose();

protected:
    std::vector<std::unique_ptr<Widget>> m_widgets;

    template<typename T, typename... Args>
    T& addWidget(Args&&... args) {
        auto widget = std::make_unique<T>(
            std::forward<Args>(args)...
        );

        T& reference = *widget;

        m_widgets.push_back(std::move(widget));

        return reference;
    }
};