#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::~Chunk() {
	if (hasMesh)
		chunkMesh.free();

	hasMesh = false;
}