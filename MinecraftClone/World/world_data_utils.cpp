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

std::unordered_map<glm::ivec3, BlockType, ivec3hash> fillChunkAsync(const glm::ivec3 chunkPos, const int chunkSize, const std::vector<int>& heightMap) {
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				int height = heightMap[z + x * chunkSize];
				if (blockPos.y > height)
					continue;

				// blocks[blockPos] = BlockType::NYCELIUM;
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

Stamp makeTreeStamp() {
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	
	const auto height = 14;
	for (int i = 0; i < height; ++i) {
		blocks[{ 0, i, 0 }] = BlockType::ASH_LOG;
	}

	const auto size = 1;
	for (int x = -size; x <= size; ++x) {
		for (int y = -size; y <= size; ++y) {
			for (int z = -size; z <= size; ++z) {
				blocks[{ x, height + y, z }] = BlockType::GLOW_BERRIES;
			}
		}
	}

	return { blocks };
}
