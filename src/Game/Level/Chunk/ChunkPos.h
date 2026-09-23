#pragma once

#include <cstddef>
#include <functional>

struct ChunkPos {
	int x;
	int z; 

	bool operator==(const ChunkPos& other) const {
		return x == other.x && z == other.z;
	}
};

struct ChunkPosHash {
	std::size_t operator()(const ChunkPos& pos) const {
		std::size_t hx = std::hash<int>{}(pos.x);
		std::size_t hz = std::hash<int>{}(pos.z);

		return hx ^ (hz << 1);
	}
};