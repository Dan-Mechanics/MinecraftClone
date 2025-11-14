#include "vao.h"

vao::vao() {
	glGenVertexArrays(1, &id);
}

void vao::linkVbo(vbo vbo, GLuint layout) const {
	vbo.bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void vao::bind() const {
	glBindVertexArray(id);
}

void vao::unbind() {
	glBindVertexArray(0);
}

void vao::free() const {
	glDeleteVertexArrays(1, &id);
}