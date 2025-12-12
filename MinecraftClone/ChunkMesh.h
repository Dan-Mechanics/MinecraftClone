#pragma once
#include <string>
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>
#include "ChunkVBO.h"

class ChunkMesh {
public:
	std::vector<ChunkVertex> vertices{};
	std::vector<GLuint> indices{};
	glm::mat4 modelMatrix{};
	VAO vao{};
	EBO ebo{};
	ChunkVBO vbo{};

	ChunkMesh();
	ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices);
	ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices, const glm::mat4& modelMatrix);

	void drawShadowChunk(const Shader& shader, const Camera& camera) const;

	void drawChunk(const Shader& shader, const Camera& camera,
		const glm::vec3& lightPos, const glm::vec4& lightColor,
		const glm::vec4& worldColor, const std::vector<Texture>& material) const;

	void free() const;

};