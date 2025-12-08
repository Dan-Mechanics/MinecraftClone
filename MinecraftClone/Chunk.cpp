#include "Chunk.h"

Chunk::Chunk(const BlockPos& chunkPos) {
	// HERE WE WANT TO GENERATE A NEW CHUNK DATA MEME
	// FUTURE: USE PERLIN THAT IS CONSISTENT.

	// THE GAG IS THAT WE LOAD AND UNLOAD CHUNKS INTO RAM
	// DYNAMICALLY BASED ON RENDER DISTANCE ISH.
}

Chunk::~Chunk() {
	mesh.free();
}

void Chunk::draw() const {
	//mesh.draw
}

void Chunk::reloadMesh() {

}
