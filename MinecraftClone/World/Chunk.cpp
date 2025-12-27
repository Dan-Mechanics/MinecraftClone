#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::Chunk(const glm::ivec3& chunkPos, const int chunkSize) : chunkPos{ chunkPos } { }

Chunk::~Chunk() {
	if (hasMesh)
		chunkMesh.free();
}