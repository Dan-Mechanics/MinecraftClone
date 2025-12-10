#pragma once
#include <unordered_map>
#include "Object.h"
#include "BlockType.h"
#include "Atlas.h"
#include "Direction.h"
#include "utils.h"
#include "Util.h"
#include "world_mesh_utils.h"

class Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, hasing_utils::HashVec3> blocks{};
	glm::ivec3 chunkPos{};
	bool hasMeshed{};
	Mesh mesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos, const unsigned int chunkSize);
	virtual ~Chunk();

	void drawShadows(Object& chunkObject, const Shader& shader, const Camera& camera) const;
	void draw(Object& chunkObject, const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) const;

	void generateMesh(const Atlas& atlas, const std::unordered_map<glm::ivec3, Chunk, hasing_utils::HashVec3>& chunks);

private:
	void fillBlocks(const unsigned int chunkSize);

};