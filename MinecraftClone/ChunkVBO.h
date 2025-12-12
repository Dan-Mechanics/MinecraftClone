#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct ChunkVertex {
public:
	glm::ivec3 position{};
	glm::vec3 normal{};
	glm::vec2 texUv{};

	ChunkVertex() = default;
	ChunkVertex(const glm::vec3& position)
		: position(position) { }

	ChunkVertex(const glm::vec3& position, const glm::vec3& normal)
		: position(position), normal(normal) { }
	
	ChunkVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texUv)
		: position(position), normal(normal), texUv(texUv) { }

};

class ChunkVBO {
public:
	GLuint id{};
	ChunkVBO();
	ChunkVBO(const std::vector<ChunkVertex>& vertices);

	void bind() const;
	void unbind() const;
	void free() const;
};