#include "Chunk.h"

#include "../../../Engine/Graphics/Mesh/CubeMesh.h"
#include "../../../Engine/Physics/Transform.h"

#include "../Level.h"
#include "../Block/Block.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

Chunk::Chunk(Level& level, int x, int z) 
	: m_level(level),
	m_x(x),
	m_z(z),
	m_bb({
		{ x * Size, 0, z * Size },
		{ (x + 1) * Size, Height, (z + 1) * Size }
	}),
	m_blocks(std::make_unique<u8[]>(Size * Height * Size)) {

	siv::PerlinNoise* perlin = level.getPerlinNoise();

	constexpr int SEA_LEVEL = 8;

	for (int x = 0; x < Size; x++) {
		for (int z = 0; z < Size; z++) {
			int worldX = m_x * Size + x;
			int worldZ = m_z * Size + z;

			float continental = perlin->octave2D_01(
				worldX * 0.0035,
				worldZ * 0.0035,
				4
			);

			float mountains = perlin->octave2D_01(
				worldX * 0.012,
				worldZ * 0.012,
				5
			);

			float detail = perlin->octave2D_01(
				worldX * 0.045,
				worldZ * 0.045,
				3
			);

			float mountainShape = glm::pow(mountains, 4.0f);

			float mountainMask = glm::smoothstep(
				0.32f,
				0.58f,
				continental
			);

			float ridge =
				1.0f - glm::abs(mountains * 2.0f - 1.0f);
			ridge = glm::pow(ridge, 2.0f);

			float baseHeight =
				6.0f +
				continental * 24.0f;

			float mountainHeight =
				mountainShape *
				mountainMask *
				100.0f;

			float ridgeHeight =
				ridge *
				mountainMask *
				70.0f;

			float detailHeight = (detail - 0.5f) * 8.0f;

			int height = static_cast<int>(
				baseHeight +
				mountainHeight +
				ridgeHeight +
				detailHeight
			);

			height = glm::clamp(
				height,
				1,
				Height - 1
			);

			for (int y = 0; y < height; y++) {
				int depth = height - 1 - y;

				if (height < SEA_LEVEL + 2) {
					// Underwater terrain
					if (depth <= 2) {
						setBlock(x, y, z, Block::Sand.getId());
					}
					else {
						setBlock(x, y, z, Block::Stone.getId());
					}
				}
				else {
					// Land terrain
					if (depth == 0) {
						setBlock(x, y, z, Block::Grass.getId());
					}
					else if (depth <= 2) {
						setBlock(x, y, z, Block::Dirt.getId());
					}
					else {
						setBlock(x, y, z, Block::Stone.getId());
					}
				}

				for (int y = height; y < SEA_LEVEL; y++) {
					setBlock(x, y, z, Block::Water.getId());
				}
			}
		}
	}

	//trees
	for (int x = 2; x < Size - 2; x++) {
		for (int z = 2; z < Size - 2; z++) {
			int surfaceY = getSurfaceHeight(x, z);

			if (surfaceY <= 0 || surfaceY >= Height - 7)
				continue;

			if (getBlock(x, surfaceY - 1, z) != Block::Grass.getId())
				continue;

			int worldX = m_x * Size + x;
			int worldZ = m_z * Size + z;

			float treeNoise = perlin->octave2D_01(
				worldX * 0.18,
				worldZ * 0.18,
				2
			);

			if (treeNoise > 0.72f) {
				generateTree(x, surfaceY, z);
			}
		}
	}


}

void Chunk::generateTree(int x, int y, int z) {
	constexpr int trunkHeight = 5;

	// Trunk
	for (int i = 0; i < trunkHeight; i++) {
		setBlock(
			x,
			y + i,
			z,
			Block::Log.getId()
		);
	}

	int leavesY = y + trunkHeight - 2;

	// Leaves
	for (int dy = 0; dy <= 3; dy++) {
		int radius = (dy == 3) ? 1 : 2;

		for (int dx = -radius; dx <= radius; dx++) {
			for (int dz = -radius; dz <= radius; dz++) {
				// Slightly round the canopy
				if (
					std::abs(dx) == radius &&
					std::abs(dz) == radius &&
					dy > 0
					) {
					continue;
				}

				int bx = x + dx;
				int by = leavesY + dy;
				int bz = z + dz;

				if (getBlock(bx, by, bz) == Block::Air.getId()) {
					setBlock(
						bx,
						by,
						bz,
						Block::Leaves.getId()
					);
				}
			}
		}
	}
}

int Chunk::getSurfaceHeight(int x, int z) const {
	for (int y = Height - 1; y >= 0; y--) {
		if (getBlock(x, y, z) != Block::Air.getId()) {
			return y + 1;
		}
	}

	return 0;
}

void Chunk::setBlock(int x, int y, int z, u32 id) {
	if (x < 0 || y < 0 || z < 0 || x >= Size || y >= Height || z >= Size) {
		return;
	}
	int index = x + (y * Size) + (z * Size * Height);
	m_blocks[index] = id;
}

u32 Chunk::getBlock(int x, int y, int z) const {
	if (x < 0 || y < 0 || z < 0 || x >= Size || y >= Height || z >= Size) {
		return 0;
	}
	int index = x + (y * Size) + (z * Size * Height);
	return m_blocks[index];
}

void Chunk::rebuild() {
	ChunkMeshData data = buildMeshData();
	uploadMesh(std::move(data));
}

void Chunk::render(Renderer& renderer) {
	if (m_dirty) {
		rebuild();
		m_dirty = false;
	}

	if (m_mesh) {
		glm::vec3 pos = glm::vec3(
			static_cast<float>(m_x * Chunk::Size),
			0.0f,
			static_cast<float>(m_z * Chunk::Size)
		);

		Transform transform = { pos };
		renderer.draw(*m_mesh, transform);
	}
}

bool Chunk::shouldRenderFaceLocal(
	u32 currentId,
	int x,
	int y,
	int z,
	Face face
) const {
	if (y < 0 || y >= Height)
		return true;

	u32 neighborId;

	if (x >= 0 && x < Size &&
		z >= 0 && z < Size) {

		// Same chunk
		neighborId = getBlock(x, y, z);
	}
	else {
		// Crossed chunk border
		int worldX = m_x * Size + x;
		int worldZ = m_z * Size + z;

		neighborId = m_level.getBlock(
			worldX,
			y,
			worldZ
		);
	}

	if (currentId == Block::Water.getId()) {
		if (neighborId == Block::Water.getId())
			return false;

		return neighborId == Block::Air.getId();
	}

	if (neighborId == Block::Air.getId())
		return true;

	if (neighborId == currentId)
		return false;

	return !Block::get(neighborId).isOpaque();
}

ChunkMeshData Chunk::buildMeshData() const {
	ChunkMeshData data;

	u32 opaqueIndexOffset = 0;
	u32 transparentIndexOffset = 0;

	for (int x = 0; x < Size; x++) {
		for (int y = 0; y < Height; y++) {
			for (int z = 0; z < Size; z++) {
				u32 blockId = getBlock(x, y, z);

				if (blockId == Block::Air.getId()) {
					continue;
				}

				const Block& block = Block::get(blockId);

				bool isWater = blockId == Block::Water.getId();

				MeshData& meshData =
					isWater
					? data.transparent
					: data.opaque;

				u32& indexOffset =
					isWater
					? transparentIndexOffset
					: opaqueIndexOffset;

				auto addFace = [&](Face face, int nx, int ny, int nz) {
					if (!shouldRenderFaceLocal(
						blockId,
						nx,
						ny,
						nz,
						face
					)) {
						return;
					}

					bool isWater =
						blockId == Block::Water.getId();

					float blockHeight =
						isWater ? 0.875f : 1.0f;

					CubeMesh::addFace(
						meshData.vertices,
						meshData.texCoords,
						meshData.normals,
						meshData.indices,
						glm::vec3(x, y, z),
						face,
						indexOffset,
						glm::vec3(1.0f),
						block.getRegion(face),
						m_level.getAtlas(),
						blockHeight
					);

					indexOffset += 4;
					};

				addFace(Face::Left, x - 1, y, z);
				addFace(Face::Right, x + 1, y, z);
				addFace(Face::Top, x, y + 1, z);
				addFace(Face::Bottom, x, y - 1, z);
				addFace(Face::Front, x, y, z + 1);
				addFace(Face::Back, x, y, z - 1);
			}
		}
	}

	return data;
}

void Chunk::uploadMesh(ChunkMeshData&& data) {
	if (data.opaque.vertices.empty()) {
		m_mesh.reset();
	}
	else {
		m_mesh = std::make_unique<Mesh>(
			data.opaque.vertices,
			data.opaque.texCoords,
			data.opaque.normals,
			data.opaque.indices,
			GL_TRIANGLES
		);
	}

	if (data.transparent.vertices.empty()) {
		m_transparentMesh.reset();
	}
	else {
		m_transparentMesh = std::make_unique<Mesh>(
			data.transparent.vertices,
			data.transparent.texCoords,
			data.transparent.normals,
			data.transparent.indices,
			GL_TRIANGLES
		);
	}

	m_dirty = false;
}

void Chunk::renderTransparent(Renderer& renderer) {
	if (!m_transparentMesh)
		return;

	Transform transform{
		glm::vec3(
			static_cast<float>(m_x * Size),
			0.0f,
			static_cast<float>(m_z * Size)
		)
	};

	renderer.draw(
		*m_transparentMesh,
		transform
	);
}