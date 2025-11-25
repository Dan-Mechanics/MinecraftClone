#include "vbo.h"

vbo::vbo() = default;

vbo::vbo(const std::vector<Vertex>& vertices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void vbo::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vbo::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo::free() const {
	glDeleteBuffers(1, &id);
}