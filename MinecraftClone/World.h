#pragma once
#include <vector>
#include "Mesh.h"
#include "Object.h"
#include <queue>
#include "world_mesh.h"

#define CHUNK_SIZE 16
#define MAP std::vector<std::vector<glm::vec2>>
#define ATLAS std::unordered_map<BlockType, MAP>

class World {
public:
	World();
	World(const float maxViewingRange);

	void draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera, const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor);
	void drawForShadowMap(const Shader& shader, const Camera& camera);
	
	void fill();
	void tick();
	void add(const BlockPos & blockPos, const BlockType & blockType);
	void remove(const BlockPos & blockPos);
	void flush(const ATLAS& atlas);
	void clear();
	void free() const;

private:
	std::unordered_map<BlockPos, glm::vec3> chunkCenters{};
	std::unordered_set<BlockPos> changedChunkPositions{};
	std::unordered_map<BlockPos, Mesh> chunkMeshes{};
	float maxViewingRange{};
	WORLD_DATA* worldData{};
	Object chunkObject{};

	const BlockPos up = { 0, 1, 0 };
	const BlockPos down = { 0, -1, 0 };
	const BlockPos left = { -1, 0, 0 };
	const BlockPos right = { 1, 0, 0 };
	const BlockPos forward = { 0, 0, 1 };
	const BlockPos back = { 0, 0, -1 };
	const glm::vec3 chunkPosOffset{
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f
	};

	void notifyChunkChange(const BlockPos& chunkPos);
	void updateChunkMesh(const BlockPos& chunkPos, const ATLAS& atlas);

};