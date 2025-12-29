#include "Chunk.h"

Chunk::Chunk() = default;

Chunk::~Chunk() {
	//std::cout << "chunk destroyed" << std::endl;
	if (hasMesh)
		chunkMesh.free();

	hasMesh = false;
}