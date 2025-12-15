#pragma once
#include <unordered_map>
#include "Object.h"
#include "BlockType.h"
#include "utils.h"
#include "hashing_utils.h"
#include "ChunkMesh.h"

struct Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, vec3hash> blocks{};
	ChunkMesh chunkMesh{};
	glm::ivec3 chunkPos{};
	bool hasMesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos, const int chunkSize);
	virtual ~Chunk();

};