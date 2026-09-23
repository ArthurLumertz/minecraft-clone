#pragma once

#include "Texture/Texture.h"

#include "Shader/StaticShader.h"

#include "../Physics/Transform.h"

#include "Mesh/Mesh.h"
#include "Mesh/QuadMesh.h"
#include "Mesh/OutlineCubeMesh.h"

class Renderer {
private:
	int m_width;
	int m_height;

	QuadMesh m_quadMesh;
	OutlineCubeMesh m_outlineCubeMesh;

	Camera* m_camera;
	StaticShader* m_shader;

	Texture* m_boundTexture;

public:
	Renderer(int width, int height);

	void begin(Camera& camera, StaticShader& shader);
	void draw(Mesh& mesh, const Transform& transform);
	void drawBillboard(Mesh& mesh, const Transform& transform);
	
	void bindTexture(Texture& texture);

	void drawTexture(float x, float y, float width, float height);
	void drawTexture(const Region& region, float x, float y, float width, float height);

	void drawOutlineCube(const glm::ivec3& blockPos);

	int getWidth() const { return m_camera->getViewportWidth(); }
	int getHeight() const { return m_camera->getViewportHeight(); }

	StaticShader* getShader() const { return m_shader; }

};