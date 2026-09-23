#pragma once

#include "Mesh.h"
#include <glm/glm.hpp>

struct Region;
class Texture;

enum class Face {
	Left, Right, Top, Bottom, Front, Back
};

class CubeMesh : public Mesh {
private:
	static const std::vector<float> vertices;
	static const std::vector<float> texCoords;
	static const std::vector<float> normals;
	static const std::vector<uint32_t> indices;

	static glm::vec2 toAtlasUV(
		const glm::vec2& uv,
		const Region& region,
		const Texture& texture
	);
public:
	CubeMesh();

	static void addFace(std::vector<float>& vertices,
		std::vector<float>& texCoords,
		std::vector<float>& normals,
		std::vector<uint32_t>& indices,
		const glm::vec3& position,
		Face face,
		uint32_t indexOffset,
		const glm::vec3& color,
		const Region& region,
		const Texture& atlas,
		float height);
};