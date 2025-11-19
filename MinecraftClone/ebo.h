#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class ebo {
public:
	GLuint id;
	ebo();
	ebo(std::vector<GLuint>& indices);

	void bind() const;
	void unbind() const;
	void free() const;
};

#endif