#pragma once

#include <glm/glm.hpp>

enum class ProjectionType {
	Perspective,
	Orthographic
};

class Camera {
private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionViewMatrix;
	glm::vec3 m_position;

	ProjectionType m_type;
	
	float m_yaw;
	float m_pitch;

	float m_viewportWidth;
	float m_viewportHeight;
	float m_near;
	float m_far;
	float m_fov;

public:
	Camera(float viewportWidth, float viewportHeight, float fovy, ProjectionType type);
	
	void recalculateMatrices();
	
	void setViewport(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; updateProjectionMatrix(); recalculateMatrices(); }

	void setPitch(float pitch) { m_pitch = pitch; recalculateMatrices(); }
	void setYaw(float yaw) { m_yaw = yaw; recalculateMatrices(); }
	void setRotation(float yaw, float pitch) { m_yaw = yaw; m_pitch = pitch; recalculateMatrices(); }

	void addYaw(float yaw) { m_yaw += yaw; m_yaw = glm::mod(m_yaw, 360.0f); recalculateMatrices(); }
	void addPitch(float pitch) { m_pitch += pitch; m_pitch = glm::clamp(m_pitch, -89.9f, 89.9f); recalculateMatrices(); }

	void updateProjectionMatrix();

	void setFov(float fov) { m_fov = fov; updateProjectionMatrix(); }
	float getFov() const { return m_fov; }

	float getViewportWidth() { return m_viewportWidth; }
	float getViewportHeight() { return m_viewportHeight; }

	void addPosition(const glm::vec3& position) { m_position += position; }
	void setPosition(const glm::vec3& position) { m_position = position; }
	const glm::vec3& getPosition() const { return m_position; }

	const glm::mat4& getProjectionViewMatrix() { recalculateMatrices(); return m_projectionViewMatrix; }
	const glm::mat4& getViewMatrix() { recalculateMatrices(); return m_viewMatrix; }

	const glm::vec3 getForward() const;
	const glm::vec3 getRight() const;
	const glm::vec3 getRayDirection() const { return getForward(); }

};