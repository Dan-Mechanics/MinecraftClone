#include "vao.h"

VAO::VAO() {
	glGenVertexArrays(1, &id);
}

void VAO::linkAttribute(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) const {
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VAO::bind() const {
	glBindVertexArray(id);
}

void VAO::unbind() const {
	glBindVertexArray(0);
}

void VAO::free() const {
	glDeleteVertexArrays(1, &id);
}