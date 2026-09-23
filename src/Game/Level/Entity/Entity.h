#pragma once

#include "../../../Engine/Physics/AABB.h"
#include "../../../Engine/Physics/Transform.h"
#include <glm/glm.hpp>
#include <vector>

class Level;

class Entity {
public:
	static constexpr float GRAVITY = 40.0f;
	static constexpr float JUMP_HEIGHT = 1.25f;

	Entity(Level* level, const glm::vec3& position);
	virtual ~Entity() = default;

	virtual void update(float delta) = 0;

	void move(glm::vec3 movement);
	void moveRelative(const glm::vec3& movement, float speed);

	bool isOnGround() const { return m_onGround; }

	Transform& getTransform() { return m_transform; }

	glm::vec3 getVelocity() { return m_velocity; }

	glm::vec3 getForward();
	glm::vec3 getRight();

	Level* getLevel() const { return m_level; }

protected:
	Level* m_level;

	Transform m_transform;

	AABB m_bb;
	glm::vec3 m_bbSize{ 0.6f, 1.8f, 0.6f };

	glm::vec3 m_velocity;

	float m_maxSpeed;
	float m_speed;

	bool m_onGround;

	void resetPos();
	void setScale(const glm::vec3& scale);
	void setPos(const glm::vec3& position);

private:
	std::vector<AABB> m_aABBs;
};