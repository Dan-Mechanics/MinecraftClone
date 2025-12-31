#include "world_data_utils.h"

std::vector<int> getHeightMap(const FastNoiseLite& noise, const float height, const int chunkX, const int chunkZ, const int chunkSize) {
	std::vector<int> result(chunkSize * chunkSize);
	auto xOffset = chunkX * chunkSize;
	auto zOffset = chunkZ * chunkSize;

	auto index = 0;
	for (int x = 0; x < chunkSize; ++x) {
		for (int z = 0; z < chunkSize; ++z) {
			result[index++] = noise.GetNoise((float)x + xOffset, (float)z + zOffset) * height;
		}
	}

	return result;
}

bool fillChunk(const glm::ivec3& chunkPos, const int chunkSize, const std::vector<int>& heightGrid, std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks) {
	auto hasChanged = false;
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				int height = heightGrid[z + x * chunkSize];
				if (blockPos.y > height)
					continue;

				if (!hasChanged)
					chunks[chunkPos] = new Chunk{};

				chunks[chunkPos]->blocks[blockPos] = static_cast<BlockType>(abs(y) % BlockType::END);
				hasChanged = true;
			}
		}
	}

	return hasChanged;
}