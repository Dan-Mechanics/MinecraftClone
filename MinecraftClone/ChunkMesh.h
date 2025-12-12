#pragma once
#include <string>
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>

class ChunkMesh {
public:
	std::vector<Vertex> vertices{};
	std::vector<GLuint> indices{};
	glm::mat4 modelMatrix{};
	VAO vao{};
	EBO ebo{};
	VBO vbo{};

	ChunkMesh();
	ChunkMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);
	ChunkMesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const glm::mat4& modelMatrix);

	void drawShadows(const Shader& shader, const Camera& camera) const;

	void draw(const Shader& shader, const Camera& camera,
		const glm::vec3& lightPos, const glm::vec4& lightColor,
		const glm::vec4& worldColor, const std::vector<Texture>& material) const;

	void free() const;

};