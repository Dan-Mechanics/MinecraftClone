#include "World.h"

World::World() = default;
World::World(const WORLD& world, const float maxViewingRange) : maxViewingRange{ maxViewingRange } {
	this->world = world;
	chunkObject = { glm::vec3{ 0.0f }, glm::vec3{ 0.0f }, glm::vec3{ 1.0f } };
}

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

	auto it = world.begin();
	while (it != world.end()) {
		generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, it->second, world);
		chunkMeshes.emplace_back(chunkVerts, chunkTris, chunkMatrix);
		chunkPositions.emplace_back(it->first.getVec3() * (float)CHUNK_SIZE);

		logVec3(chunkPositions[chunkPositions.size() - 1]);
		++it;
	}
}