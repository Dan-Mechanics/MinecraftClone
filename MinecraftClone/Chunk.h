#pragma once
#include <unordered_map>
#include "Object.h"
#include "BlockType.h"
#include "utils.h"
#include "hashing_utils.h"

class Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, vec3hash> blocks{};
	glm::ivec3 chunkPos{};
	bool hasMesh{};
	Mesh mesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos, const int chunkSize);
	virtual ~Chunk();


private:
	void generateChunkData(const int chunkSize);

};