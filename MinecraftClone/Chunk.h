#pragma once
#include "Mesh.h"
#include "world_mesh.h"

#define CHUNK_DATA std::unordered_map<BlockPos, BlockType>
#define WORLD_DATA std::unordered_map<BlockPos, CHUNK_DATA>

#define MAP std::vector<std::vector<glm::vec2>>
#define ATLAS std::unordered_map<BlockType, MAP>

#define CHUNK_SIZE 16

class Chunk {
public:
	Mesh mesh{};
	CHUNK_DATA chunkData{};
	Chunk(const BlockPos& chunkPos);
	virtual ~Chunk();
	void draw() const;
	void reloadMesh();
};