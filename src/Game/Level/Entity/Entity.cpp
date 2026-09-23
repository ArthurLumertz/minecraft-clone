#include "Entity.h"
#include "../Level.h"
#include "../Chunk/Chunk.h"
#include <glm/glm.hpp>

Entity::Entity(Level* level, const glm::vec3& position): m_level(level) {
    setPos(position);
}

void Entity::update(float delta) {
}

void Entity::resetPos() {
    int x = static_cast<int>(
        glm::floor(m_transform.position.x)
    );

    int z = static_cast<int>(
        glm::floor(m_transform.position.z)
    );

    int y = Chunk::Height - 1;

    // Find highest solid block at current X/Z
    while (y > 0 && m_level->getBlock(x, y, z) == 0) {
        y--;
    }

    // Spawn on top of it
    y++;

    setPos({
        static_cast<float>(x) + 0.5f,
        static_cast<float>(y),
        static_cast<float>(z) + 0.5f
        });

    // Important if resetPos() happens after falling/dying
    m_velocity = glm::vec3(0.0f);
}

void Entity::setScale(const glm::vec3& scale) {
    m_bbSize = scale;
    m_bb.min = m_transform.position - m_bbSize * 0.5f;
    m_bb.max = m_transform.position + m_bbSize * 0.5f;
}

void Entity::setPos(const glm::vec3& position) {
    m_transform.position = position;
    m_bb.min = position - m_bbSize * 0.5f;
    m_bb.max = position + m_bbSize * 0.5f;
}

void Entity::move(glm::vec3 movement) {
    glm::vec3 original = movement;

    m_aABBs.clear();
    m_level->getCollisionBoxes(
        this,
        m_bb.expand(movement),
        m_aABBs
    );

    for (const AABB& aABB : m_aABBs) {
        movement.y = aABB.clipYCollide(m_bb, movement.y);
    }
    m_bb.move({ 0.0f, movement.y, 0.0f });
    if (movement.y != original.y) {
        m_velocity.y = 0.0f;
    }

    m_onGround =
        original.y < 0.0f &&
        movement.y != original.y;

    for (const AABB& aABB : m_aABBs) {
        movement.x = aABB.clipXCollide(m_bb, movement.x);
    }
    m_bb.move({ movement.x, 0.0f, 0.0f });
    if (movement.x != original.x) {
        m_velocity.x = 0.0f;
    }

    for (const AABB& aABB : m_aABBs) {
        movement.z = aABB.clipZCollide(m_bb, movement.z);
    }
    m_bb.move({ 0.0f, 0.0f, movement.z });
    if (movement.z != original.z) {
        m_velocity.z = 0.0f;
    }
    
    m_transform.position = {
        (m_bb.min.x + m_bb.max.x) * 0.5f,
        m_bb.min.y,
        (m_bb.min.z + m_bb.max.z) * 0.5f
    };
}

void Entity::moveRelative(const glm::vec3& movement, float speed) {
    if (glm::dot(movement, movement) < 0.01f) {
        return;
    }

    glm::vec3 forward = getForward();
    forward.y = 0.0f;
    forward = glm::normalize(forward);

    glm::vec3 right = getRight();

    glm::vec3 input = glm::normalize(movement);

    glm::vec3 direction =
        forward * input.z +
        right * input.x;

    m_velocity.x = direction.x * speed;
    m_velocity.z = direction.z * speed;
}

glm::vec3 Entity::getForward() {
    float pitchRad = glm::radians(m_transform.rotation.x);
    float yawRad = glm::radians(m_transform.rotation.y);

    glm::vec3 front;
    front.x = glm::cos(pitchRad) * glm::cos(yawRad);
    front.y = glm::sin(pitchRad);
    front.z = glm::cos(pitchRad) * glm::sin(yawRad);
    front = glm::normalize(front);
    return front;
}

glm::vec3 Entity::getRight() {
    glm::vec3 right;
    right = glm::cross(getForward(), glm::vec3(0.0f, 1.0f, 0.0f));
    right = glm::normalize(right);
    return right;
}