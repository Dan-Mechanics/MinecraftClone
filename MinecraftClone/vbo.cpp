#include "vbo.h"

vbo::vbo(GLfloat* vertices, GLsizeiptr size) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void vbo::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vbo::unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void vbo::free() const {
	glDeleteBuffers(GL_ARRAY_BUFFER, &id);
}