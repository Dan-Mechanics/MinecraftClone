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

std::unordered_map<glm::ivec3, BlockType, ivec3hash> fillChunk(const glm::ivec3 chunkPos,
	const int chunkSize, const int waterHeight, const std::vector<int>& heightMap) {
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	for (int x = 0; x < chunkSize; ++x) {
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				int height = heightMap[z + x * chunkSize];

				if (blockPos.y <= waterHeight)
					blocks[blockPos] = BlockType::WATER;

				if (blockPos.y > height)
					continue;

				// blocks[blockPos] = BlockType::DIRT;
				blocks[blockPos] = static_cast<BlockType>(abs(y) % BlockType::WATER);
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

Stamp makeTreeStamp(const int height, const int probability, BlockType log, BlockType leaves) {
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	for (int i = 0; i < height; ++i) {
		blocks[{ 0, i, 0 }] = log;
	}

	for (int i = height; i < height + 3; ++i) {
		blocks[{ 0, i, 0 }] = leaves;
	}

	for (int i = height - 3; i < height; ++i) {
		blocks[{ 1, i, 0 }] =  leaves;
		blocks[{ 0, i, 1 }] =  leaves;
		blocks[{ -1, i, 0 }] = leaves;
		blocks[{ 0, i, -1 }] = leaves;
	}

	for (int i = height - 6; i < height - 3; ++i) {
		blocks[{ 2, i, 0 }] =  leaves;
		blocks[{ 0, i, 2 }] =  leaves;
		blocks[{ -2, i, 0 }] = leaves;
		blocks[{ 0, i, -2 }] = leaves;

		blocks[{ 1, i, 1 }] =   leaves;
		blocks[{ -1, i, 1 }] =  leaves;
		blocks[{ 1, i, -1 }] =  leaves;
		blocks[{ -1, i, -1 }] = leaves;
	}

	for (int i = height - 11; i < height - 6; ++i) {
		blocks[{ 2, i, -1 }] =  leaves;
		blocks[{ -1, i, 2 }] =  leaves;
		blocks[{ -2, i, -1 }] = leaves;
		blocks[{ -1, i, -2 }] = leaves;

		blocks[{ 2, i, 1 }] =  leaves;
		blocks[{ 1, i, 2 }] =  leaves;
		blocks[{ -2, i, 1 }] = leaves;
		blocks[{ 1, i, -2 }] = leaves;
	}

	for (int i = height - 8; i < height - 6; ++i) {
		blocks[{ 2, i, 0 }] =  leaves;
		blocks[{ 0, i, 2 }] =  leaves;
		blocks[{ -2, i, 0 }] = leaves;
		blocks[{ 0, i, -2 }] = leaves;
	}

	return { probability, false, blocks };
}

glm::ivec3 getStandardStampOrigin(const glm::ivec3& chunkPos, const int chunkSize,
	const std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash>& heightMaps) {
	auto origin = chunkPos * chunkSize;
	const auto x = randomInclusive(0, chunkSize - 1);
	const auto z = randomInclusive(0, chunkSize - 1);
	origin += glm::ivec3{ x, 0, z };
	origin.y = heightMaps.at(flatten(chunkPos))[z + x * chunkSize] + 1;

	return origin;
}