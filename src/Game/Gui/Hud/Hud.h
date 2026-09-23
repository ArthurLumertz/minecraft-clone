#pragma once

class Renderer;
class Camera;
class Texture;

class Hud {
public:
    Hud(int width, int height, Texture& atlas);

    void update(float delta);
    void render(Renderer& renderer, Camera& camera);

private:
    const int m_width;
    const int m_height;

    Texture& m_atlas;

    void renderCrosshair(Renderer& renderer);
    void renderHotbar(Renderer& renderer);
};