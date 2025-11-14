#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class ebo {
public:
	GLuint id;
	ebo(GLuint* indices, GLsizeiptr size);

	void bind() const;
	void unbind() const;
	void free() const;
};

#endif