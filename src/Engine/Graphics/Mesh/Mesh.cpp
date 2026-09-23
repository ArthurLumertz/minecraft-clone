#include "Mesh.h"
#include <glad/glad.h>

unsigned int Mesh::m_lastVaoId;

Mesh::Mesh(
	const std::vector<float>& vertices,
	const std::vector<float>& texCoords,
	const std::vector<float>& normals,
	const std::vector<uint32_t>& indices,
	int type
) : 
	m_indexCount(indices.size()),
	m_type(type) {

	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	glGenBuffers(1, &m_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_tboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_tboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_nboId);
	glBindBuffer(GL_ARRAY_BUFFER, m_nboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normals.size(), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &m_vboId);
	glDeleteBuffers(1, &m_tboId);
	glDeleteBuffers(1, &m_nboId);
	glDeleteBuffers(1, &m_eboId);
	glDeleteVertexArrays(1, &m_vaoId);
}

void Mesh::render() {
	if (m_lastVaoId != m_vaoId) {
		glBindVertexArray(m_vaoId);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		m_lastVaoId = m_vaoId;
	}

	glDrawElements(m_type, m_indexCount, GL_UNSIGNED_INT, (void*)0);
}