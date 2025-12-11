#pragma once
#include <unordered_map>
#include "Object.h"
#include "BlockType.h"
#include "utils.h"
#include "hashing_utils.h"

class Chunk {
public:
	std::unordered_map<glm::ivec3, BlockType, vec3hash> blocks{};
	glm::ivec3 chunkPos{};
	bool hasMesh{};
	Mesh mesh{};

	Chunk();
	Chunk(const glm::ivec3& chunkPos, const int chunkSize);
	virtual ~Chunk();

	void drawShadows(Object& chunkObject, const Shader& shader, const Camera& camera) const;
	void draw(Object& chunkObject, const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) const;

private:
	void generateChunkData(const int chunkSize);

};