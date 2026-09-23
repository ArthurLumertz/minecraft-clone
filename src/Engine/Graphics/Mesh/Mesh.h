#pragma once

#include <vector>
#include <cstdint>

class Mesh {
private:
	unsigned int m_vaoId;
	unsigned int m_vboId;
	unsigned int m_tboId;
	unsigned int m_nboId;
	unsigned int m_eboId;

	int m_type;
	size_t m_indexCount;

	static unsigned int m_lastVaoId;

public:
	Mesh(const std::vector<float>& vertices,
		 const std::vector<float>& texCoords,
		 const std::vector<float>& normals,
		 const std::vector<uint32_t>& indices,
		 int type);
	~Mesh();

	void render();

	const size_t getIndexCount() const { return m_indexCount; }
};