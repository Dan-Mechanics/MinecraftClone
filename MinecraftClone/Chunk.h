#pragma once
#include "Mesh.h"
#include "world_mesh.h"
#include "Object.h"

#define CHUNK_SIZE 16
#define MAP std::vector<std::vector<glm::vec2>>
#define ATLAS std::unordered_map<BlockType, MAP>

class Chunk {
public:
	Mesh mesh{};
	std::unordered_map<BlockPos, BlockType> data{};
	BlockPos chunkPos{};

	Chunk(const BlockPos& chunkPos);
	virtual ~Chunk();

	void drawForShadowMap(Object & chunkObject, const Shader& shader, const Camera& camera) const;
	void draw(Object& chunkObject, const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) const;

	void reloadMesh(const ATLAS& atlas, const std::unordered_map<BlockPos, Chunk*>& chunks);
};