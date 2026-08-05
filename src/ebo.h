#pragma once
#include "minecraft_clone.h"
#include <vector>

class ebo {
public:
	GLuint id;
	ebo();
	ebo(const std::vector<GLuint>& indices);

	void bind() const;
	void unbind() const;
	void free() const;
};