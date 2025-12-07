#include "World.h"

World::World() = default;
World::World(const WORLD& world, const float maxViewingRange)
	: maxViewingRange{ maxViewingRange }, world{ world } { }

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	for (size_t i = 0; i < chunkMeshes.size(); ++i) {
		if (glm::length(camera.position - chunkPositions[i]) > maxViewingRange)
			continue;

		// CHUNK IS VISIBLE.
		chunkObject.drawWithMaterial(chunkMeshes[i], material, shader, camera, lightColor, lightPos, worldColor);
	}
}

void World::drawForShadowMap(const Shader& shader, const Camera& camera) {
	for (size_t i = 0; i < chunkMeshes.size(); ++i) {
		if (glm::length(camera.position - chunkPositions[i]) > maxViewingRange)
			continue;

		// CHUNK IS VISIBLE.
		chunkObject.drawAsUnlitColor(chunkMeshes[i], shader, camera);
	}
}

void World::generateChunkMeshes(const ATLAS& atlas) {
	std::vector<Vertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};
	glm::mat4 chunkMatrix{};

	const glm::vec3 chunkPosOffset {
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f,
		(float)CHUNK_SIZE / 2.0f
	};

	auto it = world.begin();
	while (it != world.end()) {
		generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, it->second, world);
		chunkMeshes.emplace_back(chunkVerts, chunkTris, chunkMatrix);
		chunkPositions.emplace_back(it->first.getVec3() * (float)CHUNK_SIZE + chunkPosOffset);
		++it;
	}
}

void World::free() const {
	auto it = chunkMeshes.begin();
	while (it != chunkMeshes.end()) {
		it->free();
		++it;
	}
}