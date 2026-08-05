#pragma once
#include "minecraft_clone.h"
#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class ChunkMesh {
public:
	std::vector<ChunkVertex> vertices{};
	std::vector<GLuint> indices{};
	VAO vao{};
	EBO ebo{};
	VBO vbo{};
	bool hasData{};

	ChunkMesh();
	ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices);
	void draw() const;
	void free();

};