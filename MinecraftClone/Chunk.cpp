#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos, const int chunkSize) : chunkPos{ chunkPos } {
	//generateChunkData(chunkSize);
	//std::cout << "created: " << std::flush;
	//logIvec3(chunkPos);
}

Chunk::~Chunk() {
	if (hasMesh)
		chunkMesh.free();

	//std::cout << "destroyed: " << std::flush;
	//logIvec3(chunkPos);
}