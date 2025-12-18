#include "world_data_utils.h"

bool doesChunkGenerateBlocks(const glm::ivec3& chunkPos) {
	if (chunkPos.x == 0 && chunkPos.y == 0 && chunkPos.z == 0)
		return true;

	if (chunkPos.y == -1)
		return true;

	return false;
}

void fillChunkData(std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize, const glm::ivec3& chunkPos) {
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
		for (int y = 0; y < chunkSize; ++y) {
			for (int z = 0; z < chunkSize; ++z) {
				if (randomInclusive(0, 1))
					continue;

				const glm::ivec3 blockPos = glm::ivec3{ x, y, z } + chunkPos * chunkSize;
				const auto blockType = y >= 14 ? BlockType::NYCELIUM : BlockType::DIRT;
				// const auto blockType = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));

				chunks[chunkPos]->blocks[blockPos] = blockType;
			}
		}
	}
}

void getWorldChanges(std::vector<glm::ivec3>& chunksToNotify, const glm::ivec3& playerChunkPos,
	const std::unordered_set<glm::ivec3, ivec3hash>& visibleArea, std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	// REMOVE OLD. ===
	auto it1 = chunks.begin();
	while (it1 != chunks.end()) {
		if (!visibleArea.contains(it1->first - playerChunkPos)) {
			chunksToNotify.emplace_back(it1->first);
			//notifyChunkChange(it1->first);
			delete it1->second;
			it1 = chunks.erase(it1);
			continue;
		}

		++it1;
	}

	// ADD NEW. ===
	auto it2 = visibleArea.begin();
	while (it2 != visibleArea.end()) {
		const glm::ivec3 chunkPos = *it2 + playerChunkPos;
		if (!chunks.contains(chunkPos) && doesChunkGenerateBlocks(chunkPos)) {
			chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
			fillChunkData(chunks, chunkSize, chunkPos);
			chunksToNotify.emplace_back(chunkPos);
		}

		++it2;
	}
}
