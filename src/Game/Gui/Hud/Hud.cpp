#include "Hud.h"
#include <glm/glm.hpp>
#include "../../../Engine/Graphics/Renderer.h"

Hud::Hud(int width, int height, Texture& atlas) : 
	m_width(width),
	m_height(height),
	m_atlas(atlas) {

}

void Hud::update(float delta) {

}

void Hud::render(Renderer& renderer, Camera& camera) {
	renderer.bindTexture(m_atlas);

	renderCrosshair(renderer);
	renderHotbar(renderer);
}

void Hud::renderCrosshair(Renderer& renderer) {
	float size = 6.0f;
	float x = renderer.getWidth() * 0.5f - size * 0.5f;
	float y = renderer.getHeight() * 0.5f - size * 0.5f;

	renderer.drawTexture(
		{ 0, 0, 7, 7 },
		x, y,
		size, size
	);

}

void Hud::renderHotbar(Renderer& renderer) {

	
}