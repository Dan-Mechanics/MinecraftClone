#include "Chunk.h"

Chunk::Chunk() = default;
Chunk::~Chunk() {
	mesh.free();
}