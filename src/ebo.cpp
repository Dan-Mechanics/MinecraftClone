#include "EBO.h"

EBO::EBO() = default;
EBO::EBO(const std::vector<GLuint>& indices) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::free() const {
	glDeleteBuffers(1, &id);
}