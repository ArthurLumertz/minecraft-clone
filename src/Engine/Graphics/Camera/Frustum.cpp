#include "Frustum.h"
#include "../../Physics/AABB.h"

void Frustum::update(const glm::mat4& m) {
    // Left
    m_planes[0] = glm::vec4(
        m[0][3] + m[0][0],
        m[1][3] + m[1][0],
        m[2][3] + m[2][0],
        m[3][3] + m[3][0]
    );

    // Right
    m_planes[1] = glm::vec4(
        m[0][3] - m[0][0],
        m[1][3] - m[1][0],
        m[2][3] - m[2][0],
        m[3][3] - m[3][0]
    );

    // Bottom
    m_planes[2] = glm::vec4(
        m[0][3] + m[0][1],
        m[1][3] + m[1][1],
        m[2][3] + m[2][1],
        m[3][3] + m[3][1]
    );

    // Top
    m_planes[3] = glm::vec4(
        m[0][3] - m[0][1],
        m[1][3] - m[1][1],
        m[2][3] - m[2][1],
        m[3][3] - m[3][1]
    );

    // Near
    m_planes[4] = glm::vec4(
        m[0][3] + m[0][2],
        m[1][3] + m[1][2],
        m[2][3] + m[2][2],
        m[3][3] + m[3][2]
    );

    // Far
    m_planes[5] = glm::vec4(
        m[0][3] - m[0][2],
        m[1][3] - m[1][2],
        m[2][3] - m[2][2],
        m[3][3] - m[3][2]
    );

    for (glm::vec4& plane : m_planes) {
        float length = glm::length(glm::vec3(plane));

        if (length > 0.0f) {
            plane /= length;
        }
    }
}

bool Frustum::intersects(const AABB& box) const {
    for (const glm::vec4& plane : m_planes) {
        glm::vec3 normal(plane);

        glm::vec3 p(
            normal.x >= 0.0f ? box.max.x : box.min.x,
            normal.y >= 0.0f ? box.max.y : box.min.y,
            normal.z >= 0.0f ? box.max.z : box.min.z
        );

        if (glm::dot(normal, p) + plane.w < 0.0f) {
            return false;
        }
    }

    return true;
}