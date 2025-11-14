#include "ebo.h"

ebo::ebo(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
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