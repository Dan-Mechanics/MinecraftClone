#pragma once
#include <unordered_set>
#include "shader.h"
#include "Camera.h"
#include "Texture.h"
#include "world_mesh_utils.h"
#include "ThreadPool.h"

class World {
public:

	World();
	World(const int chunkSize, const int renderRadius);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void tick(ThreadPool& pool, const glm::vec3 & playerPos);
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void flushAll(ThreadPool& pool, const Atlas& atlas);
	void refreshSingleChunkMesh(ThreadPool& pool, const Atlas& atlas);
	void free();

private:
	std::unordered_map<glm::ivec3, Chunk*, vec3hash> chunks{};
	std::unordered_set<glm::ivec3, vec3hash> changedChunkPositions{};
	int chunkSize{};
	int renderRadius{};
	//Object chunkObject{};

	void notifyChunkChange(const glm::ivec3& chunkPos);

};