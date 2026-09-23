#pragma once

#include "../../Engine/Graphics/Renderer.h"
#include "../../Engine/Graphics/Texture/Texture.h"
#include "../../Engine/Graphics/Camera/Frustum.h"

#include "Level.h"
#include "LevelListener.h"
#include "Chunk/Chunk.h"
#include <memory>

class LevelRenderer : public LevelListener {
private:
	Level& m_level;
	int m_lastChunkX;
	int m_lastChunkZ;

	Frustum m_frustum;

	Texture m_sunTexture;
	QuadMesh m_sunQuad;
	
public:
	LevelRenderer(Level& level);
	~LevelRenderer();

	void update(Camera& camera);
	void render(Renderer &renderer);

	void renderSun(Renderer& renderer, Camera& camera);
	glm::vec3 getSunDirection() const;
	float getDaylight() const;
	glm::vec3 getSkyColor() const;

	void onBlockChange(int x, int y, int z) override;

	static constexpr i8 RenderDistance = 8;
};