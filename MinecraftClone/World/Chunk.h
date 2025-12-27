#pragma once
#include <unordered_map>
#include "BlockType.h"
#include "hashing_utils.h"
#include "ChunkMesh.h"

struct Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	ChunkMesh* chunkMesh{};
	glm::ivec3 chunkPos{};
	bool hasMesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos, const int chunkSize);
	virtual ~Chunk();

};