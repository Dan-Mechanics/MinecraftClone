#include "world_data_utils.h"

std::optional<glm::ivec3> fillChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize, const glm::ivec3 chunkPos) {
	if (chunkPos.y != 0)
		return std::nullopt;

	chunks[chunkPos] = new Chunk{};
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				if (randomInclusive(0, 1) == 0)
					continue;

				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				chunks[chunkPos]->blocks[blockPos] = static_cast<BlockType>(randomInclusive(0, BlockType::END - 1));
			}
		}
	}

	return std::optional<glm::ivec3>{ chunkPos };
}