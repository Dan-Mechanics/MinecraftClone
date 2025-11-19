#include "ebo.h"

ebo::ebo() = default;

ebo::ebo(std::vector<GLuint>& indices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void ebo::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ebo::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void ebo::free() const {
	glDeleteBuffers(1, &id);
}