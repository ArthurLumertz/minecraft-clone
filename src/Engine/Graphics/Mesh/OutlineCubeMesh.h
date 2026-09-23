#pragma once

#include "Mesh.h"

class OutlineCubeMesh : public Mesh {
private:
	static const std::vector<float> vertices;
	static const std::vector<float> texCoords;
	static const std::vector<float> normals;
	static const std::vector<uint32_t> indices;

public:
	OutlineCubeMesh();
};