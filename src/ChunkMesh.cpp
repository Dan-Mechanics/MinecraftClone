#include "ChunkMesh.h"

ChunkMesh::ChunkMesh() = default;
ChunkMesh::ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices) {
	this->vertices = vertices;
	this->indices = indices;

	vao.bind();
	vbo = { vertices };
	ebo = { indices };

	vao.linkAttribute(vbo, 0, 3, GL_INT, sizeof(ChunkVertex), (void*)0);
	vao.linkAttribute(vbo, 1, 3, GL_INT, sizeof(ChunkVertex), (void*)(3 * sizeof(int)));
	vao.linkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(ChunkVertex), (void*)(6 * sizeof(int)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();

	hasData = true;
}

void ChunkMesh::draw() const {
	vao.bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void ChunkMesh::free() {
	if (!hasData)
		return;

	vao.free();
	vbo.free();
	ebo.free();

	hasData = false;
}