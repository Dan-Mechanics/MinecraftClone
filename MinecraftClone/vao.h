#pragma once
#include <glad/glad.h>
#include "vbo.h"

class vao {
public:
	GLuint id;
	vao();

	void linkAttribute(vbo& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) const;
	void bind() const;
	void unbind() const;
	void free() const;
};