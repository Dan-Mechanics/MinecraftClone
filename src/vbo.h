#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>
#include "Vertex.h"
#include "ChunkVertex.h"

class vbo {
public:
	GLuint id{};
	vbo();
	vbo(const std::vector<Vertex>& vertices);
	vbo(const std::vector<ChunkVertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};