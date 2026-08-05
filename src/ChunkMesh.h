#pragma once
#include "minecraft_clone.h"
#include "vao.h"
#include "ebo.h"
#include "Camera.h"
#include "Texture.h"

class ChunkMesh {
public:
	std::vector<ChunkVertex> vertices{};
	std::vector<GLuint> indices{};
	vao vao{};
	ebo ebo{};
	vbo vbo{};
	bool hasData{};

	ChunkMesh();
	ChunkMesh(const std::vector<ChunkVertex>& vertices, const std::vector<GLuint>& indices);
	void draw() const;
	void free();

};