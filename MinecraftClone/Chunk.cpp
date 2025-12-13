#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos, const int chunkSize) : chunkPos{ chunkPos } {
	generateChunkData(chunkSize);
	/*std::cout << "created: " << std::flush;
	log(chunkPos);*/
}

Chunk::~Chunk() {
	if (hasMesh)
		chunkMesh.free();

	/*std::cout << "destroyed: " << std::flush;
	log(chunkPos);*/
}

void Chunk::generateChunkData(const int chunkSize) {
	// FOR THE TIME BEING.
	if (chunkPos.y != -1)
		return;
	
	for (int x = 0; x < chunkSize; x++) {
		for (int y = 0; y < chunkSize; y++) {
			/*for (int z = 0; z < chunkSize; z++) {
				if (randomInclusive(0, 1))
					blocks[glm::ivec3{ x, y, z } + chunkPos * chunkSize] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
			}*/

			blocks[glm::ivec3{ x, 0, y } + chunkPos * chunkSize] = BlockType::GRAVEL;

			if (randomInclusive(0, 1))
				blocks[glm::ivec3{ x, 1, y } + chunkPos * chunkSize] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
		}
	}
}
