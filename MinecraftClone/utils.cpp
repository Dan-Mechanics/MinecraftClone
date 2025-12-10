#include "utils.h"

int randomInclusive(const int min, const int max) {
	return rand() % (max - min + 1) + min;
}

void log(const glm::vec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void log(const glm::ivec3& vec) {
	std::cout << vec.x << " " << vec.y << " " << vec.z << std::endl;
}

void freeMaterial(const std::vector<Texture>& material) {
	auto it = material.begin();
	while (it != material.end()) {
		it->free();
		++it;
	}
}

glm::ivec3 posToBlockPos(const glm::vec3& pos) {
	return glm::ivec3{ (int)pos.x, (int)pos.y, (int)pos.z };
}

glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const unsigned int chunkSize) {
	return { blockPos.x / chunkSize, blockPos.y / chunkSize, blockPos.z / chunkSize };
}

bool isChunkValid(const glm::ivec3& chunkPos, const std::unordered_map<glm::ivec3, Chunk>& chunks) {
	return chunks.contains(chunkPos) &&
		chunks.at(chunkPos).blocks.begin() != chunks.at(chunkPos).blocks.end();
}

bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk>& chunks, const unsigned int chunkSize) {
	const auto chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos))
		return false;

	return chunks.at(chunkPos).blocks.contains(blockPos);
}
