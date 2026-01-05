#pragma once
#include "../Rendering/Shader.h"
#include "../Rendering/Camera.h"
#include "world_mesh_utils.h"
#include "world_data_utils.h"
#include "../Core/ThreadPool.h"
#include "../Core/AxisPlane.h"
#include "../Core/Raycast.h"
#include "../Core/Timer.h"
#include "WorldGenerationSettings.h"
#include "WorldSettings.h"

class World {
public:
	World();
	World(const WorldSettings& worldSettings);

	// ===

	void update(const float deltaTime, const Atlas& atlas, ThreadPool& pool, const glm::vec3& playerPos);
	void drawShadows(const Shader& shader, const Camera& camera);

	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);

	void drawTranslucent(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
		const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);

	// ===

	void add(const glm::ivec3& blockPos, const BlockType& blockType);
	void remove(const glm::ivec3& blockPos);
	void reloadChunkMesh(ThreadPool& pool, const Atlas& atlas);

	int getChunkSize() const;
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& getChunks();

	/// <summary>
	/// https://github.com/Isti01/glCraft/blob/main/src/Math/WorldRayCast.cpp
	/// </summary>
	bool raycast(const Raycast& raycast, glm::ivec3& blockPos, glm::ivec3& normal) const;
	void applyStamp(const Stamp& stamp, const glm::ivec3& origin);
;	void free();

private:
	std::unordered_map<glm::ivec3,
		std::vector<std::pair<glm::ivec3, BlockType>>, ivec3hash> pending{};

	std::unordered_set<glm::ivec3, ivec3hash> changedChunkPositions{};
	std::unordered_map<glm::ivec3, Chunk*, ivec3hash> chunks{};
	std::vector<glm::ivec3> chunkPosCache{};

	Timer reloadChunkMeshTimer{};
	Timer updateRendDistTimer{};
	bool addNewChunksMode{};

	WorldGenerationSettings worldGen{};
	WorldSettings settings{};

	void addNewChunks(ThreadPool& pool, const glm::vec3 & playerPos);
	void removeOldChunks(ThreadPool& pool, const glm::vec3 & playerPos);
	void notifyBlockChange(const glm::ivec3& chunkPos, glm::ivec3  blockPos);
	void notifyChunkChange(const glm::ivec3& chunkPos);

};