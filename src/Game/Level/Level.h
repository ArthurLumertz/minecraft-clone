#pragma once

#include "../../Engine/Core/Types.h"
#include "../../Engine/Graphics/Texture/Texture.h"

#include "PerlinNoise.h"

#include "Chunk/Chunk.h"
#include "Chunk/ChunkPos.h"

#include "BlockHit.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>

struct AABB;
class Entity;
class LevelListener;

struct ChunkLoadRequest {
	int x;
	int z;
};

struct CompletedChunk {
	std::unique_ptr<Chunk> chunk;
	ChunkMeshData meshData;
};

class Level {
public:
	Level();
	~Level();

	void addEntity(std::unique_ptr<Entity> entity);
	void removeEntity(Entity& entity);

	void addLevelListener(LevelListener* levelListener);

	u32 getBlock(int x, int y, int z) const;
	void setBlock(int x, int y, int z, u32 id);
	bool isBlock(int x, int y, int z) const;

	bool shouldRenderFace(u32 id, int x, int y, int z) const;

	void update(float delta);

	// Synchronous loading.
	// Keep this mostly for startup/debugging.
	Chunk& loadChunk(int x, int z);

	// Async loading.
	void requestChunk(int x, int z);

	// Call once per frame from the main thread.
	void processLoadedChunks();

	Chunk* getChunk(int x, int z) const;

	void unloadChunk(int x, int z);
	void unloadChunksOutside(int centerX, int centerZ, int distance);

	void getCollisionBoxes(
		const Entity* entity,
		const AABB& region,
		std::vector<AABB>& out
	) const;

	bool raycastBlocks(
		const glm::vec3& origin,
		const glm::vec3& direction,
		float maxDistance,
		BlockHit& hit
	) const;

	Texture& getAtlas() {
		return m_atlas;
	}

	siv::PerlinNoise* getPerlinNoise() {
		return &m_perlin;
	}

	const auto& getChunks() const {
		return m_chunks;
	}

	float getTimeOfDay() const { return m_timeOfDay; }

private:
	void chunkWorker();

private:
	float m_timeOfDay;

	std::vector<AABB> m_boxes;
	std::vector<std::unique_ptr<Entity>> m_entities;

	// MAIN THREAD ONLY
	std::unordered_map<
		ChunkPos,
		std::unique_ptr<Chunk>,
		ChunkPosHash
	> m_chunks;

	siv::PerlinNoise m_perlin;
	Texture m_atlas;

	// -------------------------
	// Async chunk generation
	// -------------------------

	std::thread m_chunkThread;

	std::mutex m_chunkMutex;
	std::condition_variable m_chunkCV;

	std::queue<ChunkLoadRequest> m_chunkRequests;
	std::queue<CompletedChunk> m_completedChunks;

	// Prevent requesting the same chunk 50 times
	std::unordered_set<ChunkPos, ChunkPosHash> m_pendingChunks;

	std::atomic<bool> m_chunkThreadRunning{ true };

	int worldToChunk(int value) const;
	int worldToLocal(int value) const;
};