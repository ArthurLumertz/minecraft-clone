#pragma once

#include "../../../Engine/Core/Types.h"
#include "../../../Engine/Physics/AABB.h"
#include "../../../Engine/Graphics/Mesh/CubeMesh.h"
#include "../../../Engine/Graphics/Renderer.h"
#include "ChunkPos.h"

#include <memory>

class Level;

struct MeshData {
	std::vector<float> vertices;
	std::vector<float> texCoords;
	std::vector<float> normals;
	std::vector<u32> indices;
};

struct ChunkMeshData {
	MeshData opaque;
	MeshData transparent;
};

class Chunk {
private:
	Level& m_level;

	std::unique_ptr<u8[]> m_blocks;

	int m_x;
	int m_z;

	AABB m_bb;

	bool m_dirty = true;

	std::unique_ptr<Mesh> m_mesh;
	std::unique_ptr<Mesh> m_transparentMesh;

	int getSurfaceHeight(int x, int z) const;
	void generateTree(int x, int y, int z);

public:
	static constexpr int Size = 16;
	static constexpr int Height = 128;

	Chunk(Level &level, int x, int z);

	u32 getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, u32 id);

	void rebuild();
	void render(Renderer& renderer);

	ChunkMeshData buildMeshData() const;
	void uploadMesh(ChunkMeshData&& data);

	bool shouldRenderFaceLocal(
		u32 currentId,
		int x,
		int y,
		int z,
		Face face
	) const;

	void renderTransparent(Renderer& renderer);

	int getX() const { return m_x; }
	int getZ() const { return m_z; }

	void setDirty() { m_dirty = true; }
	const AABB& getAABB() { return m_bb; }

};