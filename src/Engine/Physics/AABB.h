#pragma once

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 min{ 0.0f };
    glm::vec3 max{ 0.0f };

    AABB() = default;

    AABB(const glm::vec3& min, const glm::vec3& max)
        : min(min), max(max) {
    }

    void set(const glm::vec3& min, const glm::vec3& max) {
        this->min = min;
        this->max = max;
    }

    AABB expand(const glm::vec3& movement) const {
        AABB result = *this;

        if (movement.x < 0.0f) result.min.x += movement.x;
        else result.max.x += movement.x;

        if (movement.y < 0.0f) result.min.y += movement.y;
        else result.max.y += movement.y;

        if (movement.z < 0.0f) result.min.z += movement.z;
        else result.max.z += movement.z;

        return result;
    }

    void move(const glm::vec3& movement) {
        min += movement;
        max += movement;
    }

    bool intersects(const AABB& other) const {
        return
            max.x > other.min.x &&
            min.x < other.max.x &&
            max.y > other.min.y &&
            min.y < other.max.y &&
            max.z > other.min.z &&
            min.z < other.max.z;
    }

    float clipXCollide(const AABB& other, float xa) const {
        if (
            other.max.y <= min.y ||
            other.min.y >= max.y ||
            other.max.z <= min.z ||
            other.min.z >= max.z
            ) {
            return xa;
        }

        if (xa > 0.0f && other.max.x <= min.x) {
            float d = min.x - other.max.x;
            if (d < xa) xa = d;
        }

        if (xa < 0.0f && other.min.x >= max.x) {
            float d = max.x - other.min.x;
            if (d > xa) xa = d;
        }

        return xa;
    }

    float clipYCollide(const AABB& other, float ya) const {
        if (
            other.max.x <= min.x ||
            other.min.x >= max.x ||
            other.max.z <= min.z ||
            other.min.z >= max.z
            ) {
            return ya;
        }

        if (ya > 0.0f && other.max.y <= min.y) {
            float d = min.y - other.max.y;
            if (d < ya) ya = d;
        }

        if (ya < 0.0f && other.min.y >= max.y) {
            float d = max.y - other.min.y;
            if (d > ya) ya = d;
        }

        return ya;
    }

    float clipZCollide(const AABB& other, float za) const {
        if (
            other.max.x <= min.x ||
            other.min.x >= max.x ||
            other.max.y <= min.y ||
            other.min.y >= max.y
            ) {
            return za;
        }

        if (za > 0.0f && other.max.z <= min.z) {
            float d = min.z - other.max.z;
            if (d < za) za = d;
        }

        if (za < 0.0f && other.min.z >= max.z) {
            float d = max.z - other.min.z;
            if (d > za) za = d;
        }

        return za;
    }
};