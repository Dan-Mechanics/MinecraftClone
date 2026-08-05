#include "vao.h"

vao::vao() {
	glGenVertexArrays(1, &id);
}

void vao::linkAttribute(vbo& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) const {
	vbo.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void vao::bind() const {
	glBindVertexArray(id);
}

void vao::unbind() const {
	glBindVertexArray(0);
}

void vao::free() const {
	glDeleteVertexArrays(1, &id);
}