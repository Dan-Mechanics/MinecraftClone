#pragma once
#include <string>
#include "vao.h"
#include "ebo.h"
#include "Camera.h"
#include "Texture.h"
#include <vector>

class ChunkMesh {
public:
	std::vector<ChunkVertex> vertices{};
	std::vector<GLuint> indices{};
	vao vao{};
	ebo ebo{};
	vbo vbo{};

	ChunkMesh();
	ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices);

	void drawShadows(const Shader& shader, const Camera& camera) const;

	void draw(const Shader& shader, const Camera& camera,
		const glm::vec3& lightPos, const glm::vec4& lightColor,
		const glm::vec4& worldColor, const std::vector<Texture>& material) const;

	void free() const;

};