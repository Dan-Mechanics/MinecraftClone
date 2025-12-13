#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"
#include "ChunkVertex.h"

class VBO {
public:
	GLuint id{};
	VBO();
	VBO(const std::vector<Vertex>& vertices);
	VBO(const std::vector<ChunkVertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};