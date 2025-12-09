#pragma once
#include "Mesh.h"
#include "Object.h"
#include <unordered_map>

class Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType> blocks{};
	glm::ivec3 chunkPos{};
	Mesh mesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos);
	virtual ~Chunk();

	void drawForShadowMap(Object & chunkObject, const Shader& shader, const Camera& camera) const;
	void draw(Object& chunkObject, const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) const;

	void generateMesh(const ATLAS& atlas, const std::unordered_map<glm::ivec3, Chunk*>& chunks);
};