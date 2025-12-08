#pragma once
#include <vector>
#include "world_mesh.h"
#include "Mesh.h"
#include "Object.h"
#include <queue>

class World {
public:
	World();
	World(const WORLD_DATA& worldData, const float maxViewingRange);
	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera, const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	void drawForShadowMap(const Shader& shader, const Camera& camera);

	/// <summary>
	/// For the time being.
	/// </summary>
	void generateChunkMeshes(const ATLAS& atlas);
	void add(const BlockPos & blockPos, const BlockType & blockType);
	void remove(const BlockPos & blockPos);
	void flush();
	void free() const;

private:
	WORLD_DATA worldData{};
	std::unordered_map<BlockPos, Mesh> chunkMeshes{};
	std::unordered_map<BlockPos, glm::vec3> chunkCenters{};
	float maxViewingRange{};
	Object chunkObject{};
	std::unordered_set<BlockPos> changedChunkPositions{};

	const BlockPos up = { 0, 1, 0 };
	const BlockPos down = { 0, -1, 0 };
	const BlockPos left = { -1, 0, 0 };
	const BlockPos right = { 1, 0, 0 };
	const BlockPos forward = { 0, 0, 1 };
	const BlockPos back = { 0, 0, -1 };

	const glm::vec3 chunkPosOffset {
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f
	};

	void notifyChunkChange(const BlockPos& chunkPos);
	void updateChunkMesh(const BlockPos& chunkPos);

};