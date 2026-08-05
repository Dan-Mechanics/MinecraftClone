#include "Stamp.h"

Stamp::Stamp() = default;
Stamp::Stamp(const int probability, const bool spawnInWater, std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other)
	: probability{ probability }, spawnInWater{ spawnInWater } {
	blocks = std::move(other);
}