#include "world_data_utils.h"

void allocateChunkData(std::unordered_map<glm::ivec3, Chunk*, vec3hash>& chunks, const int chunkSize, const glm::ivec3& chunkPos) {
	if (chunkPos.x == 0 && chunkPos.y == 0 && chunkPos.z == 0) {
		for (int x = 0; x < chunkSize; ++x) {
			for (int y = 0; y < chunkSize; ++y) {
				for (int z = 0; z < chunkSize; ++z) {
					if (randomInclusive(0, 3))
						continue;

					const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
					const auto blockType = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));

					chunks[chunkPos]->blocks[blockPos] = blockType;
				}
			}
		}

		return;
	}

	if (chunkPos.y != -1)
		return;

	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < 3; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				if (randomInclusive(0, 1))
					continue;

				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				const auto blockType = y >= 14 ? BlockType::NYCELIUM : BlockType::DIRT;
				//const auto blockType = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));

				chunks[chunkPos]->blocks[blockPos] = blockType;
			}
		}
	}
}