#pragma once
#include <unordered_map>
#include "BlockType.h"
#include "hashing_utils.h"

class Stamp {
public:
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};

	Stamp();
	Stamp(std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other);
};

