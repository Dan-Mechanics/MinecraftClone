#pragma once
#include "minecraft_clone.h"
#include "BlockType.h"
#include "hashing_utils.h"
#include "ChunkMesh.h"

struct Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, ivec3hash> blocks{};
	ChunkMesh mesh{};
	ChunkMesh translucentMesh{};

	Chunk();
	virtual ~Chunk();

};