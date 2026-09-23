#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

Renderer::Renderer(int width, int height)
	: m_width(width),
	m_height(height),
	m_outlineCubeMesh(),
	m_quadMesh() {
}

void Renderer::begin(Camera& camera, StaticShader& shader) {
	m_shader = &shader;
	m_camera = &camera;

	m_shader->bind();
	m_shader->setCamera(camera);
	m_shader->setSampler(0);
	m_shader->setUVRegion(
		0.0f, 0.0f,
		1.0f, 1.0f
	);
}

void Renderer::draw(Mesh& mesh, const Transform& transform) {
	glm::mat4 modelMatrix = glm::mat4(1.0);
	modelMatrix = glm::translate(modelMatrix, transform.position);
	
	modelMatrix = glm::rotate(
		modelMatrix,
		transform.rotation.x,
		glm::vec3(1.0f, 0.0f, 0.0f)
	);
	modelMatrix = glm::rotate(
		modelMatrix,
		transform.rotation.y,
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	modelMatrix = glm::rotate(
		modelMatrix,
		transform.rotation.z,
		glm::vec3(0.0f, 0.0f, 1.0f)
	);
	modelMatrix = glm::scale(modelMatrix, transform.scale);

	m_shader->setModelMatrix(modelMatrix);
	mesh.render();
}

void Renderer::drawBillboard(
	Mesh& mesh,
	const Transform& transform
) {
	glm::mat4 model(1.0f);

	model = glm::translate(
		model,
		transform.position
	);

	const glm::mat4& view = m_camera->getViewMatrix();

	glm::vec3 cameraRight(
		view[0][0],
		view[1][0],
		view[2][0]
	);

	glm::vec3 cameraUp(
		view[0][1],
		view[1][1],
		view[2][1]
	);

	glm::vec3 cameraForward(
		view[0][2],
		view[1][2],
		view[2][2]
	);

	model[0] = glm::vec4(
		cameraRight * transform.scale.x,
		0.0f
	);

	model[1] = glm::vec4(
		cameraUp * transform.scale.y,
		0.0f
	);

	model[2] = glm::vec4(
		cameraForward * transform.scale.z,
		0.0f
	);

	m_shader->setModelMatrix(model);

	mesh.render();
}

void Renderer::bindTexture(Texture& texture) {
	texture.bind(0);
	m_boundTexture = &texture;
}

void Renderer::drawTexture(float x, float y, float width, float height) {
	m_shader->setUVRegion(0.0f, 0.0f, 1.0f, 1.0f);
	
	Transform transform;
	transform.position = glm::vec3(x, y, 0.0f);
	transform.scale = glm::vec3(width, height, 1.0f);
	draw(m_quadMesh, transform);
}

void Renderer::drawTexture(const Region& region, float x, float y, float width, float height) {
	float texW = static_cast<float>(m_boundTexture->getWidth());
	float texH = static_cast<float>(m_boundTexture->getHeight());

	float u0 = region.x / texW;
	float v0 = region.y / texH;

	float u1 = (region.x + region.width) / texW;
	float v1 = (region.y + region.height) / texH;

	m_shader->setUVRegion(u0, 1.0f - v1, u1, 1.0f - v0);

	Transform transform;
	transform.position = glm::vec3(x, y, 0.0f);
	transform.scale = glm::vec3(width, height, 1.0f);
	draw(m_quadMesh, transform);
}

void Renderer::drawOutlineCube(
	const glm::ivec3& blockPos
) {
	Transform transform;

	constexpr float padding = 0.002f;

	transform.position = glm::vec3(blockPos) -
		glm::vec3(padding);

	transform.scale = glm::vec3(
		1.0f + padding * 2.0f
	);

	m_shader->setUseSolidColor(true);
	m_shader->setSolidColor(
		0.15f, 0.15f, 0.15f, 0.65f
	);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);

	draw(m_outlineCubeMesh, transform);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_shader->setUseSolidColor(false);
}