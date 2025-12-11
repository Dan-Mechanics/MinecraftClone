#pragma once

/// <summary>
/// https://github.com/Isti01/glCraft/blob/main/src/Util/Util.h
/// </summary>
class vec3hash {
public:
    /*size_t operator()(const glm::ivec3& coord) const noexcept {
        size_t hash = coord.x;
        hash ^= coord.y + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        hash ^= coord.z + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
    };*/

	std::size_t operator()(glm::ivec3 blockPos) const {
		std::size_t seed = 3;

		blockPos.x = ((blockPos.x >> 16) ^ blockPos.x) * 0x45d9f3b;
		blockPos.x = ((blockPos.x >> 16) ^ blockPos.x) * 0x45d9f3b;
		blockPos.x = (blockPos.x >> 16) ^ blockPos.x;
		seed ^= blockPos.x + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		blockPos.y = ((blockPos.y >> 16) ^ blockPos.y) * 0x45d9f3b;
		blockPos.y = ((blockPos.y >> 16) ^ blockPos.y) * 0x45d9f3b;
		blockPos.y = (blockPos.y >> 16) ^ blockPos.y;
		seed ^= blockPos.y + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		blockPos.z = ((blockPos.z >> 16) ^ blockPos.z) * 0x45d9f3b;
		blockPos.z = ((blockPos.z >> 16) ^ blockPos.z) * 0x45d9f3b;
		blockPos.z = (blockPos.z >> 16) ^ blockPos.z;
		seed ^= blockPos.z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

		return seed;
	}

};