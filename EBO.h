#pragma once
#include "minecraft_clone.h"

class EBO {
public:
	GLuint id;
	EBO();
	EBO(const std::vector<GLuint>& indices);

	void bind() const;
	void unbind() const;
	void free() const;
};