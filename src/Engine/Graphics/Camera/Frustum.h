#pragma once

#include <glm/glm.hpp>

struct AABB;

class Frustum {
public:
	void update(const glm::mat4& matrix);

	bool intersects(const AABB& box) const;

private:
	glm::vec4 m_planes[6];
};