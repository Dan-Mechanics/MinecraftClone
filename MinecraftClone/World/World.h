#pragma once
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"
#include "world_mesh_utils.h"
#include "world_data_utils.h"
#include "../Core/ThreadPool.h"
#include "../Core/AxisPlane.h"
#include "../Core/Raycast.h"

class World {
public:
	World();

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/World/WorldGenerator.cpp
	/// </summary>
	World(const int chunkSize, const int renderDistance);

	// FUTURE: ADD DRAW OPAQUE AND TRANSPARENT HERE.

	void drawShadows(const Shader& shader, const Camera& camera);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	
	void update(const float dt, const Atlas& atlas, ThreadPool& pool, const glm::vec3& playerPos);

	/// <summary>
	/// Manage adding and removing chunks on
	/// the heap based on player positon.
	/// </summary>
	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void reloadSingleChunkMesh(ThreadPool& pool, const Atlas& atlas);

	int getChunkSize() const;
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& getChunks();

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/Math/WorldRayCast.cpp
	/// </summary>
	bool raycast(const Raycast& raycast, glm::ivec3& blockPos, glm::ivec3& normal) const;
	void applyStamp(const Stamp& stamp, const glm::ivec3& origin);
;	void free();

private:
	std::unordered_set<glm::ivec3, ivec3hash> changedChunkPositions{};
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash> chunks{};
	std::vector<glm::ivec3> chunkPosCache{};
	int renderDistance{};
	int largeRenderDistance{};
	int smallRenderDistance{};
	FastNoiseLite noise{};
	float height{};
	int chunkSize{};
	float reloadChunkInterval{};
	float updateChunksInterval{};
	float reloadChunkTimer{};
	float updateChunksTimer{};
	bool toggle{};
	int waterHeight{};
	Stamp blueTree{};
	Stamp ashTree{};

	void addNewChunksAsync(ThreadPool& pool, const glm::vec3 & playerPos);
	void removeOldChunksAsync(ThreadPool& pool, const glm::vec3 & playerPos);

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