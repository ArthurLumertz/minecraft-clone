#include "Player.h"
#include "../../Level.h"
#include "../../../../Engine/Core/Input.h"

Player::Player(Level* level) : 
    Entity(level, m_transform.position),
	m_controller(*this) {

    m_speed = 4.0f;
	m_maxSpeed = m_speed;

    resetPos();
}

void Player::update(float delta) {
    m_controller.update(delta);

    glm::vec3 direction(0.0f);

    if (Input::isKeyDown(Key::W)) direction.z += 1.0f;
    if (Input::isKeyDown(Key::S)) direction.z -= 1.0f;
    if (Input::isKeyDown(Key::A)) direction.x -= 1.0f;
    if (Input::isKeyDown(Key::D)) direction.x += 1.0f;

    if (m_onGround && Input::isKeyDown(Key::SPACE)) {
        m_velocity.y = sqrtf(2.0f * GRAVITY * JUMP_HEIGHT);
    }

    bool sprinting = Input::isKeyDown(Key::LSHIFT);

    if (sprinting) {
        if (!m_onGround) {
            m_speed = m_maxSpeed * 1.5f;
        }
        else {
            m_speed = m_maxSpeed * 1.3f;
        }
    }
    else {
        m_speed = m_maxSpeed;
    }

    moveRelative(direction, m_speed);
    move(m_velocity * delta);

    float friction = glm::pow(0.002f, delta);
    m_velocity.x *= friction;
    m_velocity.y -= GRAVITY * delta;
    m_velocity.z *= friction;
}

glm::vec3 Player::getLookDirection() const {
    float yaw = glm::radians(m_transform.rotation.y);
    float pitch = glm::radians(m_pitch);

    glm::vec3 direction;
    direction.x = glm::cos(pitch) * glm::cos(yaw);
    direction.y = glm::sin(pitch);
    direction.z = glm::cos(pitch) * glm::sin(yaw);
    return glm::normalize(direction);
}