#include "world_data_utils.h"

std::vector<int> getHeightMap(const FastNoiseLite& noise, const float height, const int chunkX, const int chunkZ, const int chunkSize) {
	// DON'T CHANGE THE FOLLOWING:
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
	auto allocated = false;
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				int height = heightGrid[z + x * chunkSize];
				if (blockPos.y > height)
					continue;

				if (!allocated)
					chunks[chunkPos] = new Chunk{};

				chunks[chunkPos]->blocks[blockPos] = static_cast<BlockType>(abs(y) % BlockType::END);
				allocated = true;
			}
		}
	}

	return allocated;
}

std::unordered_map<glm::ivec3, BlockType, ivec3hash> fillChunkAsync(const glm::ivec3 chunkPos, const int chunkSize, const std::vector<int>& heightMap) {
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				int height = heightMap[z + x * chunkSize];
				if (blockPos.y > height)
					continue;

				blocks[blockPos] = static_cast<BlockType>(abs(y) % BlockType::END);
			}
		}
	}

	return blocks;
}

std::optional<glm::ivec3> checkKeepChunkLoaded(const glm::ivec3 chunkPos, const glm::ivec3& playerChunkPos, const int renderDistance) {
	if (chunkPos.x > playerChunkPos.x + renderDistance ||
		chunkPos.x < playerChunkPos.x - renderDistance ||
		chunkPos.z > playerChunkPos.z + renderDistance ||
		chunkPos.z < playerChunkPos.z - renderDistance ||
		chunkPos.y > playerChunkPos.y + renderDistance ||
		chunkPos.y < playerChunkPos.y - renderDistance) {
		return std::optional<glm::ivec3>{ chunkPos };
	}

	return std::nullopt;
}