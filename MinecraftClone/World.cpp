#include "World.h"

World::World() = default;
World::World(const unsigned int chunkSize, const float maxViewingRange) 
	: chunkSize{ chunkSize }, maxViewingRange{ maxViewingRange } { 
}

void World::drawShadows(const Shader& shader, const Camera& camera) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second.drawShadows(chunkObject, shader, camera);
		++it;
	}
}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second.draw(chunkObject, material, shader,
			camera, lightColor, lightPos, worldColor);

		++it;
	}
}

void World::tick(const glm::vec3& playerPos) {
}

void World::add(const glm::ivec3& blockPos, const BlockType& blockType) {
	std::cout << "add" << std::endl;
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (chunks.contains(chunkPos) && chunks[chunkPos].blocks.contains(blockPos))
		return;
	
	chunks[chunkPos].blocks[blockPos] = blockType;
	notifyChunkChange(chunkPos);
}

void World::remove(const glm::ivec3& blockPos) {
	std::cout << "remove" << std::endl;
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos) || !chunks[chunkPos].blocks.contains(blockPos))
		return;

	chunks[chunkPos].blocks.erase(blockPos);
	notifyChunkChange(chunkPos);
}

void World::flush(const Atlas& atlas) {
	std::cout << "flush" << std::endl;
	auto it = changedChunkPositions.begin();
	while (it != changedChunkPositions.end()) {
		const glm::ivec3 chunkPos = *it;
		if (!chunks.contains(chunkPos)) {
			++it;
			continue;
		}
		
		Chunk& chunk = chunks[chunkPos];

		// DELETE THE OLD MESH.
		if (chunk.hasMesh)
			chunk.mesh.free();

		std::vector<Vertex> chunkVerts{};
		std::vector<GLuint> chunkTris{};
		glm::mat4 chunkMatrix{};

		generateChunkMesh(chunkVerts, chunkTris, chunkMatrix, atlas, chunk, chunks);
		chunk.mesh = { chunkVerts, chunkTris, chunkMatrix };
		chunk.hasMesh = true;

		++it;
	}

	changedChunkPositions.clear();
}

void World::removeAll() {
	std::cout << "remove all" << std::endl;
	auto it = chunks.begin();
	while (it != chunks.end()) {
		changedChunkPositions.insert(it->first);
		it->second.blocks.clear();
		++it;
	}
}

void World::notifyChunkChange(const glm::ivec3& chunkPos) {
	changedChunkPositions.insert(chunkPos);
	changedChunkPositions.insert(chunkPos + up);
	changedChunkPositions.insert(chunkPos + down);
	changedChunkPositions.insert(chunkPos + left);
	changedChunkPositions.insert(chunkPos + right);
	changedChunkPositions.insert(chunkPos + forward);
	changedChunkPositions.insert(chunkPos + back);
}