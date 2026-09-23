#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(
    float viewportWidth,
    float viewportHeight,
    float fovy,
    ProjectionType type
)
    : m_type(type),
    m_position(0.0f),
    m_pitch(0.0f),
    m_yaw(-90.0f),
    m_viewportWidth(viewportWidth),
    m_viewportHeight(viewportHeight),
    m_fov(fovy) {

    updateProjectionMatrix();
}

void Camera::recalculateMatrices() {
    if (m_type == ProjectionType::Perspective) {
        glm::vec3 front = getForward();
        m_viewMatrix = glm::lookAt(m_position, m_position + front, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (m_type == ProjectionType::Orthographic) {
        m_viewMatrix = glm::translate(glm::mat4(1.0), -m_position);
    }

    m_projectionViewMatrix = m_projectionMatrix * m_viewMatrix;
}

void Camera::updateProjectionMatrix() {
    if (m_type == ProjectionType::Perspective) {
        m_projectionMatrix = glm::perspective(
            glm::radians(m_fov),
            m_viewportWidth / m_viewportHeight,
            0.03f,
            300.0f
        );
    }
    else {
        m_projectionMatrix = glm::ortho(
            0.0f,
            m_viewportWidth,
            0.0f,
            m_viewportHeight
        );
    }
    recalculateMatrices();
}

const glm::vec3 Camera::getForward() const {
    float pitchRad = glm::radians(m_pitch);
    float yawRad = glm::radians(m_yaw);

    glm::vec3 front;
    front.x = glm::cos(pitchRad) * glm::cos(yawRad);
    front.y = glm::sin(pitchRad);
    front.z = glm::cos(pitchRad) * glm::sin(yawRad);
    front = glm::normalize(front);
    return front;
}

const glm::vec3 Camera::getRight() const {
    glm::vec3 right;
    right = glm::cross(getForward(), glm::vec3(0.0f, 1.0f, 0.0f));
    right = glm::normalize(right);
    return right;
}