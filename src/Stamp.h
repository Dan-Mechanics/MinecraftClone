#pragma once
#include <unordered_map>
#include "../Core/utils.h"
#include "BlockType.h"
#include "hashing_utils.h"

class Stamp {
public:
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	int probability{};
	bool spawnInWater{};

	Stamp();
	Stamp(const int probability, const bool spawnInWater,
		std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other);

};

