#include "World.h"

World::World() = default;
World::World(const float maxViewingRange) : maxViewingRange{ maxViewingRange }, worldData{} {}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	auto it = chunkMeshes.begin();
	while (it != chunkMeshes.end()) {
		if (glm::length(camera.position - chunkCenters[it->first]) > maxViewingRange) {
			++it;
			continue;
		}

		// CHUNK IS VISIBLE.
		chunkObject.drawWithMaterial(it->second, material, shader, camera, lightColor, lightPos, worldColor);
		++it;
	}
}

void World::drawForShadowMap(const Shader& shader, const Camera& camera) {
	auto it = chunkMeshes.begin();
	while (it != chunkMeshes.end()) {
		if (glm::length(camera.position - chunkCenters[it->first]) > maxViewingRange) {
			++it;
			continue;
		}

		chunkObject.drawAsUnlitColor(it->second, shader, camera);
		++it;
	}
}

void World::fill() {
	const int size = 2;
	for (int i = -size; i < size; ++i) {
		for (int j = -size; j < size; ++j) {
			for (int x = 0; x < CHUNK_SIZE; ++x) {
				for (int y = 0; y < CHUNK_SIZE; ++y) {
					for (int z = 0; z < CHUNK_SIZE; ++z) {
						if (randomInclusive(0, 2))
							continue;

						add({x + i * CHUNK_SIZE, y, z + j * CHUNK_SIZE}, y >= 14 ? BlockType::NYCELIUM : BlockType::DIRT);
					}
				}
			}
		}
	}
}

void World::add(const BlockPos& blockPos, const BlockType& blockType) {
	BlockPos chunkPos = blockPosToChunkPos(blockPos);
	if (!worldData.contains(chunkPos))
		worldData[chunkPos] = {};

	// WE ALREADY HAVE THAT BLOCK.
	if (worldData[chunkPos].contains(blockPos))
		return;

	worldData[chunkPos][blockPos] = blockType;
	notifyChunkChange(chunkPos);
}

void World::remove(const BlockPos& blockPos) {
	BlockPos chunkPos = blockPosToChunkPos(blockPos);
	if (!worldData.contains(chunkPos))
		return;

	if (!worldData[chunkPos].contains(blockPos))
		return;

	worldData[chunkPos].erase(blockPos);
	notifyChunkChange(chunkPos);
}

void World::flush(const ATLAS& atlas) {
	auto it = changedChunkPositions.begin();
	while (it != changedChunkPositions.end()) {
		const BlockPos chunkPos = *it;
		if (!isChunkValid(chunkPos, worldData)) 
			worldData.erase(chunkPos);

		updateChunkMesh(chunkPos, atlas);
		++it;
	}

	changedChunkPositions.clear();
}

void World::clear() {
	auto it = worldData.begin();
	while (it != worldData.end()) {
		changedChunkPositions.insert(it->first);
		++it;
	}

	worldData.clear();
}

void World::free() const {
	auto it = chunkMeshes.begin();
	while (it != chunkMeshes.end()) {
		it->second.free();
		++it;
	}
}

void World::notifyChunkChange(const BlockPos& chunkPos) {
	changedChunkPositions.insert(chunkPos);
	changedChunkPositions.insert(chunkPos + up);
	changedChunkPositions.insert(chunkPos + down);
	changedChunkPositions.insert(chunkPos + left);
	changedChunkPositions.insert(chunkPos + right);
	changedChunkPositions.insert(chunkPos + forward);
	changedChunkPositions.insert(chunkPos + back);
}

void World::updateChunkMesh(const BlockPos& chunkPos, const ATLAS& atlas) {
	if (!isChunkValid(chunkPos, worldData)) {
		// DESTROY THE MESH.
		if (chunkMeshes.contains(chunkPos)) {
			chunkMeshes.at(chunkPos).free();
			chunkMeshes.erase(chunkPos);
			chunkCenters.erase(chunkPos);
		}

		return;
	}

	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix{};

	generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, worldData[chunkPos], worldData);
	chunkMeshes[chunkPos] = { chunkVerts, chunkTris, chunkMatrix };
	chunkCenters[chunkPos] = chunkPos.getVec3() * (float)CHUNK_SIZE + chunkPosOffset;
}
