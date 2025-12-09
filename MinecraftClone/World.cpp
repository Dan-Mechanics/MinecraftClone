#include "World.h"

World::World() = default;
World::World(const unsigned int chunkSize, const float maxViewingRange) 
	: chunkSize{ chunkSize }, maxViewingRange{ maxViewingRange } { }

void World::drawShadows(const Shader& shader, const Camera& camera) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second->drawShadows(chunkObject, shader, camera);
		++it;
	}
}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second->draw(chunkObject, material, shader,
			camera, lightColor, lightPos, worldColor);

		++it;
	}
}


void World::tick() {
	// look around the player and see which chunks need to be loaded/ unloaded.


}

void World::add(const glm::ivec3& blockPos, const BlockType& blockType) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (chunks.contains(chunkPos) && chunks[chunkPos]->blocks.contains(blockPos))
		return;
	
	chunks[chunkPos]->blocks[blockPos] = blockType;
	notifyChunkChange(chunkPos);
}

void World::remove(const glm::ivec3& blockPos) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos) || !chunks[chunkPos]->blocks.contains(blockPos))
		return;

	chunks[chunkPos]->blocks.erase(blockPos);
	notifyChunkChange(chunkPos);
}

void World::flush(const Atlas& atlas) {
	auto it = changedChunkPositions.begin();
	while (it != changedChunkPositions.end()) {
		const glm::ivec3 chunkPos = *it;
		if (chunks.contains(chunkPos))
			chunks[chunkPos]->generateMesh(atlas, chunks);

		++it;
	}

	changedChunkPositions.clear();
}

void World::removeAll() {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		changedChunkPositions.insert(it->first);
		it->second->blocks.clear();
		++it;
	}
}

void World::free() const {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		// THIS WILL AUTOMATICALLY DELETE
		// THE MESH VIA DTOR.
		delete it->second;
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