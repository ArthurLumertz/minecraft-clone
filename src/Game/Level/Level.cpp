#include "Level.h"

#include <iostream>
#include <random>
#include <cstdint>
#include <cmath>
#include <glm/glm.hpp>

#include "LevelListener.h"
#include "Entity/Entity.h"
#include "Block/Block.h"

Level::Level() :
	m_perlin(std::random_device{}()),
	m_atlas("resources/blocks.png", false) {
	loadChunk(0, 0);

	m_chunkThread = std::thread(
		&Level::chunkWorker,
		this
	);
}

Level::~Level() {
	m_chunkThreadRunning = false;

	m_chunkCV.notify_all();

	if (m_chunkThread.joinable()) {
		m_chunkThread.join();
	}
}


void Level::update(float delta) {
	m_timeOfDay += delta / 600.0f;

	if (m_timeOfDay >= 1.0f) {
		m_timeOfDay = 0.0f;
	}
}

void Level::requestChunk(int x, int z) {
	ChunkPos pos{ x, z };

	// Already loaded
	if (m_chunks.find(pos) != m_chunks.end()) {
		return;
	}

	{
		std::lock_guard<std::mutex> lock(m_chunkMutex);

		// Already being generated
		if (m_pendingChunks.find(pos) != m_pendingChunks.end()) {
			return;
		}

		m_pendingChunks.insert(pos);

		m_chunkRequests.push({
			x,
			z
			});
	}

	m_chunkCV.notify_one();
}

void Level::chunkWorker() {
	while (m_chunkThreadRunning) {
		ChunkLoadRequest request;

		{
			std::unique_lock<std::mutex> lock(m_chunkMutex);

			m_chunkCV.wait(
				lock,
				[this]() {
					return
						!m_chunkRequests.empty() ||
						!m_chunkThreadRunning;
				}
			);

			if (!m_chunkThreadRunning) {
				break;
			}

			request = m_chunkRequests.front();
			m_chunkRequests.pop();
		}

		// CPU terrain generation
		auto chunk = std::make_unique<Chunk>(
			*this,
			request.x,
			request.z
		);

		// CPU MESH GENERATION
		ChunkMeshData meshData =
			chunk->buildMeshData();

		{
			std::lock_guard<std::mutex> lock(m_chunkMutex);

			m_completedChunks.push({
				std::move(chunk),
				std::move(meshData)
				});
		}
	}
}

void Level::processLoadedChunks() {
	constexpr int MAX_UPLOADS_PER_FRAME = 1;

	for (int i = 0; i < MAX_UPLOADS_PER_FRAME; i++) {
		CompletedChunk completed;

		{
			std::lock_guard<std::mutex> lock(m_chunkMutex);

			if (m_completedChunks.empty()) {
				break;
			}

			completed =
				std::move(m_completedChunks.front());

			m_completedChunks.pop();
		}

		ChunkPos pos{
			completed.chunk->getX(),
			completed.chunk->getZ()
		};

		// THIS is the only GPU part.
		completed.chunk->uploadMesh(
			std::move(completed.meshData)
		);

		{
			std::lock_guard<std::mutex> lock(m_chunkMutex);
			m_pendingChunks.erase(pos);
		}

		if (m_chunks.find(pos) != m_chunks.end()) {
			continue;
		}

		m_chunks.emplace(
			pos,
			std::move(completed.chunk)
		);
	}
}

void Level::addLevelListener(LevelListener* levelListener) {
	// do later
}

void Level::addEntity(std::unique_ptr<Entity> entity) {
	m_entities.push_back(std::move(entity));
}

void Level::removeEntity(Entity& entity) {
	std::erase_if(m_entities, [&](const auto& ptr) {
		return ptr.get() == &entity;
	});
}

int Level::worldToChunk(int value) const {
	return static_cast<int>(
		glm::floor(static_cast<float>(value) / Chunk::Size)
	);
}

int Level::worldToLocal(int value) const  {
	int chunk = worldToChunk(value);
	return value - chunk * Chunk::Size;
}

Chunk& Level::loadChunk(int x, int z) {
	ChunkPos pos{ x, z };
	 
	auto it = m_chunks.find(pos);
	if (it != m_chunks.end()) {
		return *it->second;
	}

	auto chunk = std::make_unique<Chunk>(*this, x, z);
	Chunk& result = *chunk;
	m_chunks.emplace(pos, std::move(chunk));
	return result;
}

Chunk* Level::getChunk(int x, int z) const {
	ChunkPos pos{ x, z };

	auto it = m_chunks.find(pos);

	if (it == m_chunks.end()) {
		return nullptr;
	}

	return it->second.get();
}

void Level::unloadChunk(int x, int z) {
	ChunkPos pos{ x, z };

	m_chunks.erase(pos);
}

u32 Level::getBlock(int x, int y, int z) const {
	if (y < 0 || y >= Chunk::Height) {
		return 0;
	}

	int chunkX = worldToChunk(x);
	int chunkZ = worldToChunk(z);

	Chunk* chunk = getChunk(chunkX, chunkZ);

	if (chunk == nullptr) {
		return 0;
	}

	int localX = worldToLocal(x);
	int localZ = worldToLocal(z);

	return chunk->getBlock(localX, y, localZ);
}

bool Level::shouldRenderFace(u32 currentId, int x, int y, int z) const {
	u32 neighborId = getBlock(x, y, z);

	if (neighborId == Block::Air.getId())
		return true;

	if (neighborId == currentId)
		return false;

	const Block& neighbor = Block::get(neighborId);

	return !neighbor.isOpaque();
}

void Level::setBlock(int x, int y, int z, u32 id) {
	if (y < 0 || y >= Chunk::Height)
		return;

	int chunkX = worldToChunk(x);
	int chunkZ = worldToChunk(z);

	Chunk* chunk = getChunk(chunkX, chunkZ);
	if (!chunk)
		return;

	int localX = worldToLocal(x);
	int localZ = worldToLocal(z);

	chunk->setBlock(localX, y, localZ, id);
	chunk->setDirty();

	if (localX == 0) {
		if (Chunk* neighbor = getChunk(chunkX - 1, chunkZ))
			neighbor->setDirty();
	}
	if (localX == Chunk::Size - 1) {
		if (Chunk* neighbor = getChunk(chunkX + 1, chunkZ))
			neighbor->setDirty();
	}
	if (localZ == 0) {
		if (Chunk* neighbor = getChunk(chunkX, chunkZ - 1))
			neighbor->setDirty();
	}
	if (localZ == Chunk::Size - 1) {
		if (Chunk* neighbor = getChunk(chunkX, chunkZ + 1))
			neighbor->setDirty();
	}
}

bool Level::isBlock(int x, int y, int z) const {
	return getBlock(x, y, z) != 0;
}

void Level::unloadChunksOutside(int centerX, int centerZ, int distance) {
	for (auto it = m_chunks.begin(); it != m_chunks.end();) {
		int dx = std::abs(it->first.x - centerX);
		int dz = std::abs(it->first.z - centerZ);

		if (dx > distance || dz > distance) {
			it = m_chunks.erase(it);
		}
		else {
			++it;
		}
	}
}

void Level::getCollisionBoxes(
	const Entity* entity,
	const AABB& region,
	std::vector<AABB>& out
) const {
	int minX = static_cast<int>(std::floor(region.min.x));
	int minY = static_cast<int>(std::floor(region.min.y));
	int minZ = static_cast<int>(std::floor(region.min.z));

	int maxX = static_cast<int>(std::floor(region.max.x));
	int maxY = static_cast<int>(std::floor(region.max.y));
	int maxZ = static_cast<int>(std::floor(region.max.z));

	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			for (int z = minZ; z <= maxZ; z++) {
				if (!isBlock(x, y, z)) {
					continue;
				}

				const Block& block = Block::get(getBlock(x, y, z));
				if (!block.isSolid()) {
					continue;
				}

				out.emplace_back(
					glm::vec3(
						static_cast<float>(x),
						static_cast<float>(y),
						static_cast<float>(z)
					),
					glm::vec3(
						static_cast<float>(x + 1),
						static_cast<float>(y + 1),
						static_cast<float>(z + 1)
					)
				);
			}
		}
	}
}

bool Level::raycastBlocks(
	const glm::vec3& origin,
	const glm::vec3& direction,
	float maxDistance,
	BlockHit& hit
) const {
	constexpr float step = 0.02f;

	glm::vec3 dir = glm::normalize(direction);

	glm::ivec3 previousBlock(
		glm::floor(origin)
	);

	for (float d = 0.0f; d <= maxDistance; d += step) {
		glm::vec3 point = origin + dir * d;

		glm::ivec3 blockPos(
			glm::floor(point)
		);

		if (getBlock(blockPos.x, blockPos.y, blockPos.z) != 0) {
			hit.position = blockPos;
			hit.distance = d;

			hit.normal = previousBlock - blockPos;

			return true;
		}

		previousBlock = blockPos;
	}

	return false;
}