#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "vbo.h"

class vao {
public:
	GLuint id;
	vao();

	void linkVbo(vbo vbo, GLuint layout) const;
	void bind() const;
	void unbind();
	void free() const;
};

#endif