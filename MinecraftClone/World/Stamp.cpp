#include "Stamp.h"

Stamp::Stamp() = default;
Stamp::Stamp(const int probability, std::unordered_map<glm::ivec3, BlockType, ivec3hash>& other)
	: probability{ probability }, blocks { std::move(other) } { }

glm::ivec3 Stamp::getStandardOrigin(const glm::ivec3& chunkPos, const int chunkSize,
	const std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash>& heightMaps) const {
	auto origin = chunkPos * chunkSize;
	const auto x = randomInclusive(0, chunkSize - 1);
	const auto z = randomInclusive(0, chunkSize - 1);
	origin += glm::ivec3{ x, 0, z };
	origin.y = heightMaps.at(flatten(chunkPos))[z + x * chunkSize] + 1;
	return origin;
}
