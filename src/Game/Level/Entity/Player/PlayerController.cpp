#include "PlayerController.h"
#include "Player.h"

#include "../../BlockHit.h"
#include "../../Block/Block.h"
#include "../../Level.h"

#include <iostream>

#include "../../../../Engine/Core/Input.h"

PlayerController::PlayerController(Player& player) : m_player(player) {
}

void PlayerController::update(float delta) {
	glm::vec2 mouseDelta = Input::getMouseDelta();
	constexpr float sensitivity = 0.1f;

	Transform& transform = m_player.getTransform();
	m_player.setPitch(
		m_player.getPitch() - mouseDelta.y * sensitivity
	);

	transform.rotation.y += mouseDelta.x * sensitivity;

	// view bob

	glm::vec3 velocity = m_player.getVelocity();

	float horizontalSpeed = glm::length(
		glm::vec2(velocity.x, velocity.z)
	);

	if (m_player.isOnGround() && horizontalSpeed > 0.1f) {
		m_bobTime += delta * horizontalSpeed * 1.2f;

		float phase = m_bobTime * 2.0f;

		float sideBob = glm::cos(phase) * 0.03f;
		float verticalBob = glm::abs(glm::sin(phase)) * 0.05f;

		glm::vec3 right = m_player.getRight();
		right.y = 0.0f;

		if (glm::dot(right, right) > 0.0f) {
			right = glm::normalize(right);
		}

		m_viewBobOffset =
			right * sideBob +
			glm::vec3(0.0f, verticalBob, 0.0f);
	}
	else {
		m_viewBobOffset = glm::vec3(0.0f);
	}

	// fov
	bool sprinting = Input::isKeyDown(Key::LSHIFT);
	float targetFov = sprinting ? 78.0f : 70.0f;
	
	m_fov = glm::mix(
		m_fov,
		targetFov,
		1.0f - glm::exp(-8.0f * delta)
	);

    updateBlockBreaking(delta);
}

void PlayerController::updateBlockBreaking(float delta) {
    Level* level = m_player.getLevel();

    glm::vec3 origin =
        m_player.getTransform().position +
        glm::vec3(0.0f, 1.8f, 0.0f);

    glm::vec3 direction = m_player.getLookDirection();

    BlockHit hit;

    u32 blockId = 0;
    float hardness = 0.0f;

    if (!level->raycastBlocks(
        origin,
        direction,
        5.0f,
        hit
    )) {
        m_hasBreakingBlock = false;
        m_hasTargetBlock = false;
        m_breakProgress = 0.0f;
        return;
    }
    else {
        blockId = level->getBlock(
            hit.position.x,
            hit.position.y,
            hit.position.z
        );
        const Block& block = Block::get(blockId);
        hardness = block.getHardness();
        if (hardness > 0.0f) {
            m_hasTargetBlock = true;
            m_targetBlock = hit.position;
        }
        else {
            m_hasTargetBlock = false;
        }
    }

    if (!Input::isMouseButtonDown(MouseButton::LEFT)) {
        m_hasBreakingBlock = false;
        m_breakProgress = 0.0f;
        return;
    }

    if (!m_hasBreakingBlock ||
        hit.position != m_breakingBlock) {

        m_breakingBlock = hit.position;
        m_breakProgress = 0.0f;
        m_hasBreakingBlock = true;
    }

    m_breakProgress += delta;

    if (hardness > 0.0f && m_breakProgress >= hardness) {
        level->setBlock(
            hit.position.x,
            hit.position.y,
            hit.position.z,
            Block::Air.getId()
        );
        m_breakProgress = 0.0f;
        m_hasBreakingBlock = false;
    }
}