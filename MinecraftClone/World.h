#pragma once
#include <unordered_set>
#include "shader.h"
#include "Camera.h"
#include "Texture.h"
#include "world_mesh_utils.h"
#include "Util.h"

class World {
public:
	std::unordered_map<glm::ivec3, Chunk, hasing_utils::HashVec3> chunks{};

	World();
	World(const unsigned int chunkSize, const float maxViewingRange);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void tick(const glm::vec3 & playerPos);
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void flush(const Atlas& atlas);
	void removeAll();

private:
	std::unordered_set<glm::ivec3> changedChunkPositions{};
	unsigned int chunkSize{};
	float maxViewingRange{};
	Object chunkObject{};

	void notifyChunkChange(const glm::ivec3& chunkPos);

};