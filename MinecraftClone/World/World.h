#pragma once
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"
#include "world_mesh_utils.h"
#include "world_data_utils.h"
#include "../Core/ThreadPool.h"
#include "../Core/AxisPlane.h"
#include "../Core/utils.h"
#include "../Core/Raycast.h"

class World {
public:
	World();

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/World/WorldGenerator.cpp
	/// </summary>
	World(const int chunkSize, const int maxRenderDistance);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	/// <summary>
	/// Manage adding and removing chunks on
	/// the heap based on player positon.
	/// </summary>
	void addInsideRenderDistance(ThreadPool& pool, const glm::vec3 & playerPos);
	void removeOutsideRenderDistance(ThreadPool& pool, const glm::vec3 & playerPos);
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void reloadSingleChunkMesh(ThreadPool& pool, const Atlas& atlas);

	int getChunkSize() const;
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& getChunks();

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/Math/WorldRayCast.cpp
	/// </summary>
	bool raycast(const Raycast& raycast, glm::ivec3& blockPos, glm::ivec3& normal) const;
;	void free();

private:
	std::unordered_set<glm::ivec3, ivec3hash> changedChunkPositions{};
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash> chunks{};
	int maxRenderDistance{};
	FastNoiseLite noise{};
	int chunkSize{};

	/// <summary>
	/// Update all surrounding chunks too.
	/// </summary>
	void notifyChunkChange(const glm::ivec3& chunkPos);

	/// <summary>
	/// Only update the surroundng chunks if
	/// the block is on the border of the chunk.
	/// </summary>
	void notifyBlockChange(const glm::ivec3& chunkPos, glm::ivec3  blockPos);

};