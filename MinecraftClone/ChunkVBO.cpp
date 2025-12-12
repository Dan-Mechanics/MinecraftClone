#include "ChunkVBO.h"

ChunkVBO::ChunkVBO() = default;

ChunkVBO::ChunkVBO(const std::vector<ChunkVertex>& vertices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ChunkVertex), vertices.data(), GL_STATIC_DRAW);
}

void ChunkVBO::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void ChunkVBO::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ChunkVBO::free() const {
	glDeleteBuffers(1, &id);
}