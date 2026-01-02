#pragma once
#include <unordered_map>
#include "../Core/utils.h"
#include "BlockType.h"
#include "hashing_utils.h"

class Stamp {
public:
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	int probability{};

	Stamp();
	Stamp(const int probability, std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other);

	glm::ivec3 getStandardOrigin(const glm::ivec3& chunkPos, const int chunkSize, const std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash>& heightMaps) const;
};

