#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class vbo {
public:
	GLuint id;
	vbo(GLfloat* vertices, GLsizeiptr size);

	void bind() const;
	void unbind() const;
	void free() const;
};

#endif