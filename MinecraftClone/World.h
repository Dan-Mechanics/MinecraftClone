#pragma once
#include <vector>
#include "Mesh.h"
#include "Object.h"
#include <queue>
#include "world_mesh_utils.h"
#include <unordered_set>

class World {
public:
	World();
	World(const unsigned int chunkSize, const float maxViewingRange);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void tick();
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void flush(const Atlas& atlas);
	void removeAll();
	void free() const;

private:
	std::unordered_set<glm::ivec3> changedChunkPositions{};
	std::unordered_map<glm::ivec3, Chunk*> chunks{};
	unsigned int chunkSize{};
	float maxViewingRange{};
	Object chunkObject{};

	void notifyChunkChange(const glm::ivec3& chunkPos);

};