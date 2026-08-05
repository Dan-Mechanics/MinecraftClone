#pragma once
#include "VBO.h"

class VAO {
public:
	GLuint id;
	VAO();

	void linkAttribute(VBO& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) const;
	void bind() const;
	void unbind() const;
	void free() const;
};