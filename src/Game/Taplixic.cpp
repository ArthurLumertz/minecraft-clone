#include "Taplixic.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Gui/Screen/PauseScreen.h"

Taplixic::Taplixic()
	: Application(960, 540, "Taplixic"),
	m_level(),
	m_levelRenderer(m_level),
	m_camera(960, 540, 70.0f, ProjectionType::Perspective),
	m_guiCamera(m_camera.getViewportWidth() / 3.0f, m_camera.getViewportHeight() / 3.0f, 0.0f, ProjectionType::Orthographic),
	m_shader("default"),
	m_guiShader("gui"),
	m_renderer(m_camera.getViewportWidth(), m_camera.getViewportHeight()),
	m_gui(m_guiCamera.getViewportWidth(), m_guiCamera.getViewportHeight()),
	m_player(&m_level) {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.8f, 1.0f, 0.0f);

	int chunkX = static_cast<int>(
		glm::floor(
			m_player.getTransform().position.x /
			static_cast<float>(Chunk::Size)
		)
		);

	int chunkZ = static_cast<int>(
		glm::floor(
			m_player.getTransform().position.z /
			static_cast<float>(Chunk::Size)
		)
		);

	for (int x = chunkX - 8; x <= chunkX + 8; x++) {
		for (int z = chunkZ - 8; z <= chunkZ + 8; z++) {
			m_level.loadChunk(x, z);
		}
	}
}

void Taplixic::update(float delta) {
	m_level.processLoadedChunks();

	if (!m_gui.hasScreen()) {
		m_player.update(delta);
	}

	moveCameraToPlayer();

	m_level.update(delta);
	m_levelRenderer.update(m_camera);

	if (Input::isKeyReleased(Key::ESCAPE)) {
		if (m_gui.hasScreen()) {
			m_gui.closeScreen();
		}
		else {
			m_gui.setScreen(
				std::make_unique<PauseScreen>(m_gui)
			);
		}
	}

	if (Input::isKeyReleased(Key::F11)) {
		m_window.toggleFullscreen();
	}
}

void Taplixic::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 skyColor = m_levelRenderer.getSkyColor();
	glClearColor(
		skyColor.r,
		skyColor.g,
		skyColor.b,
		1.0f
	);

	// world
	m_renderer.begin(m_camera, m_shader);
	m_levelRenderer.render(m_renderer);

	m_renderer.getShader()->setFogColor(skyColor);

	if (m_player.getController().hasTargetBlock()) {
		m_renderer.drawOutlineCube(
			m_player.getController().getTargetBlock()
		);
	}

	glDepthMask(GL_FALSE);

	m_levelRenderer.renderSun(m_renderer, m_camera);

	glDepthMask(GL_TRUE);

	// gui
	glDisable(GL_DEPTH_TEST);
	
	m_renderer.begin(m_guiCamera, m_guiShader);
	m_gui.render(m_renderer, m_guiCamera);

	glEnable(GL_DEPTH_TEST);
}

void Taplixic::moveCameraToPlayer() {
	const Transform& transform = m_player.getTransform();
	const PlayerController& controller = m_player.getController();

	glm::vec3 cameraPos =
		transform.position + glm::vec3(0.0f, 1.8f, 0.0f) +
		controller.getViewBobOffset();

	m_camera.setPosition(cameraPos);

	m_camera.setRotation(
		transform.rotation.y,
		m_player.getPitch()
	);

	m_camera.setFov(controller.getFov());
}

void Taplixic::onResize(int width, int height) {
	m_camera.setViewport(width, height);
	m_guiCamera.setViewport(width / 3, height / 3);
}