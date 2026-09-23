#pragma once

#include "../Entity.h"
#include "PlayerController.h"

class Player : public Entity {
private:
	PlayerController m_controller;
	float m_pitch;

public: 
	Player(Level* level);

	void update(float delta) override;

	float getPitch() { return m_pitch; }
	void setPitch(float pitch) { m_pitch = glm::clamp(pitch, -89.9f, 89.9f); }

	glm::vec3 getLookDirection() const;

	const PlayerController& getController() const { return m_controller; }
};