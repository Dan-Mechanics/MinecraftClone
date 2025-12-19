#include "world_data_utils.h"

bool fillChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize, const glm::ivec3& chunkPos) {
	if (chunkPos.x == 0 && chunkPos.y == 0 && chunkPos.z == 0) {
		chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
		
		for (int x = 0; x < chunkSize; ++x) {
			for (int y = 0; y < chunkSize; ++y) {
				for (int z = 0; z < chunkSize; ++z) {
					if (randomInclusive(0, 1))
						continue;

					const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
					// const auto blockType = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
					chunks[chunkPos]->blocks[blockPos] = BlockType::LOG;
				}
			}
		}

		return true;
	}

	if (chunkPos.y != -1)
		return false;

	chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				if (y >= 14) {
					if (randomInclusive(0, 1))
						continue;

					chunks[chunkPos]->blocks[blockPos] = BlockType::NYCELIUM;
				}
				else {
					chunks[chunkPos]->blocks[blockPos] = BlockType::DIRT;
				}
			}
		}
	}

	return true;
}