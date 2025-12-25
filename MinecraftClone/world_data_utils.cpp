#include "world_data_utils.h"

bool fillChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize, const glm::ivec3& chunkPos) {
	if (chunkPos.y != 0)
		return false;

	chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < 2; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				if (y == 0) {
					if (randomInclusive(0, 50) == 0) {
						for (int i = 0; i < chunkSize; i++) {
							chunks[chunkPos]->blocks[blockPos + glm::ivec3{ 0, i, 0 }] = BlockType::GHOST_LOG;
						}
					}
					else
					{
						chunks[chunkPos]->blocks[blockPos] = BlockType::DIRT;
					}
				}
				else if (randomInclusive(0, 1) != 0) {
					chunks[chunkPos]->blocks[blockPos] = BlockType::NYCELIUM;
				}
			}
		}
	}

	return true;
}