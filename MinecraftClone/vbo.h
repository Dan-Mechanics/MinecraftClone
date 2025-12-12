#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"

class VBO {
public:
	GLuint id{};
	VBO();
	VBO(const std::vector<Vertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};