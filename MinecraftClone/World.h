#pragma once
#include "shader.h"
#include "Camera.h"
#include "world_mesh_utils.h"
#include "world_data_utils.h"
#include "ThreadPool.h"
#include "AxisPlane.h"
#include "utils.h"

class World {
public:
	bool toggle{};
	
	World();
	World(const int chunkSize, const int maxRenderDistance);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	/// <summary>
	/// Manage adding and removing chunks on
	/// the heap based on player positon.
	/// </summary>
	void allocateNewChunks(ThreadPool& pool, const glm::vec3 & playerPos);
	void destroyOldChunks(ThreadPool& pool, const glm::vec3 & playerPos);
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void reloadSingleChunkMesh(ThreadPool& pool, const Atlas& atlas);
	
	/// <summary>
	/// CREDIT: https://github.com/Isti01/glCraft/blob/main/src/Math/WorldRayCast.cpp
	/// </summary>
	bool raycast(const glm::vec3& origin, const glm::vec3& direction, const float range, glm::ivec3& blockPos, glm::ivec3& normal) const;
;	void free();

private:
	std::unordered_set<glm::ivec3, ivec3hash> changedChunkPositions{};
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash> chunks{};
	int maxRenderDistance{};
	int chunkSize{};

	void notifyChunkChange(const glm::ivec3& chunkPos);

	/// <summary>
	/// Only update the surroundng chunks if
	/// the block is on the border of the chunk.
	/// </summary>
	void notifyBlockChange(const glm::ivec3& chunkPos, glm::ivec3  blockPos);

};