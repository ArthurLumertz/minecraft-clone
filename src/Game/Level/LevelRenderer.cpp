#include "LevelRenderer.h"
#include "Chunk/ChunkPos.h"
#include "../../Engine/Graphics/Texture/Texture.h"
#include "../../Engine/Graphics/Shader/StaticShader.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

LevelRenderer::LevelRenderer(Level& level) 
    : m_level(level),
    m_sunTexture("resources/sun.png") {
	level.addLevelListener(this);
}

LevelRenderer::~LevelRenderer() {

}

void LevelRenderer::update(Camera& camera) {
    m_frustum.update(camera.getProjectionViewMatrix());

    int chunkX = static_cast<int>(
        glm::floor(camera.getPosition().x / static_cast<float>(Chunk::Size))
    );

    int chunkZ = static_cast<int>(
        glm::floor(camera.getPosition().z / static_cast<float>(Chunk::Size))
    );

    if (chunkX == m_lastChunkX && chunkZ == m_lastChunkZ) {
        return;
    }

    m_lastChunkX = chunkX;
    m_lastChunkZ = chunkZ;

    for (int x = -RenderDistance; x <= RenderDistance; x++) {
        for (int z = -RenderDistance; z <= RenderDistance; z++) {
            m_level.requestChunk(
                chunkX + x,
                chunkZ + z
            );
        }
    }

    m_level.unloadChunksOutside(
        chunkX,
        chunkZ,
        RenderDistance
    );
}

void LevelRenderer::render(Renderer& renderer) {
    renderer.bindTexture(m_level.getAtlas());

    // -------------------------
    // Opaque terrain pass
    // -------------------------

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    for (const auto& [pos, chunk] : m_level.getChunks()) {
        if (!chunk)
            continue;

        if (!m_frustum.intersects(chunk->getAABB()))
            continue;

        chunk->render(renderer);
    }

    // -------------------------
    // Transparent / water pass
    // -------------------------

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glDepthMask(GL_FALSE);

    for (const auto& [pos, chunk] : m_level.getChunks()) {
        if (!chunk)
            continue;

        if (!m_frustum.intersects(chunk->getAABB()))
            continue;

        chunk->renderTransparent(renderer);
    }

    // Restore state
    glDepthMask(GL_TRUE);
}

void LevelRenderer::onBlockChange(int x, int y, int z) {
    int chunkX = static_cast<int>(
        glm::floor(static_cast<float>(x) / Chunk::Size)
    );

    int chunkZ = static_cast<int>(
        glm::floor(static_cast<float>(z) / Chunk::Size)
    );

    Chunk* chunk = m_level.getChunk(chunkX, chunkZ);

    if (!chunk)
        return;

    chunk->setDirty();
}

void LevelRenderer::renderSun(
    Renderer& renderer,
    Camera& camera
) {
    glm::vec3 sunDirection = getSunDirection();

    glm::vec3 sunPosition =
        camera.getPosition() +
        sunDirection * 150.0f;

    Transform transform;
    transform.position = sunPosition;
    transform.scale = glm::vec3(20.0f);

    renderer.getShader()->setUseFog(false);
    renderer.getShader()->setUseLighting(false);

    renderer.getShader()->setSunDirection(sunDirection);

    renderer.bindTexture(m_sunTexture);
    renderer.drawBillboard(
        m_sunQuad,
        transform
    );

    renderer.getShader()->setUseLighting(true);
    renderer.getShader()->setUseFog(true);
}

glm::vec3 LevelRenderer::getSunDirection() const {
    float angle = m_level.getTimeOfDay() * glm::two_pi<float>();

    return glm::normalize(
        glm::vec3(
            glm::cos(angle),
            glm::sin(angle),
            glm::sin(angle) * 0.35f
        )
    );
}

float LevelRenderer::getDaylight() const {
    glm::vec3 sunDir = getSunDirection();

    return glm::smoothstep(
        -0.15f,
        0.20f,
        sunDir.y
    );
}

glm::vec3 LevelRenderer::getSkyColor() const {
    glm::vec3 sunDir = getSunDirection();

    float daylight = getDaylight();

    glm::vec3 nightColor(
        0.015f,
        0.02f,
        0.06f
    );

    glm::vec3 dayColor(
        0.5f,
        0.8f,
        1.0f
    );

    glm::vec3 sunsetColor(
        1.0f,
        0.35f,
        0.12f
    );

    glm::vec3 sky = glm::mix(
        nightColor,
        dayColor,
        daylight
    );

    // Strongest when sun Y ~= 0
    float sunset =
        1.0f - glm::clamp(
            glm::abs(sunDir.y) / 0.25f,
            0.0f,
            1.0f
        );

    // Only really show it around sunrise/sunset
    sunset *= 1.0f - glm::abs(daylight * 2.0f - 1.0f);

    sky = glm::mix(
        sky,
        sunsetColor,
        sunset * 0.65f
    );

    return sky;
}