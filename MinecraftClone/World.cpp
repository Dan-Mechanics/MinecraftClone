#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int renderRadius)
	: chunkSize{ chunkSize }, renderRadius{ renderRadius } { }

void World::drawShadows(const Shader& shader, const Camera& camera) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second->mesh.drawShadowChunk(shader, camera);
		++it;
	}
}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		it->second->mesh.drawChunk(shader, camera, lightPos, lightColor, worldColor, material);
		++it;
	}
}

void World::tick(const glm::vec3& playerPos) {
	const auto playerBlockPos = posToBlockPos(playerPos);
	const auto playerChunkPos = blockPosToChunkPos(playerBlockPos, chunkSize);

	std::unordered_set<glm::ivec3, vec3hash> visibleArea{};
	for (int x = -renderRadius; x < renderRadius; ++x) {
		for (int y = -renderRadius; y < renderRadius; ++y) {
			for (int z = -renderRadius; z < renderRadius; ++z) {
				visibleArea.insert(playerChunkPos + glm::ivec3{ x, y, z });
			}
		}
	}

	// REMOVE OLD. ===
	auto it1 = chunks.begin();
	while (it1 != chunks.end()) {
		if (!visibleArea.contains(it1->second->chunkPos)) {
			changedChunkPositions.insert(it1->first);
			delete it1->second;
			it1 = chunks.erase(it1);
			continue;
		}

		++it1;
	}

	// ADD NEW. ===
	auto it2 = visibleArea.begin();
	while (it2 != visibleArea.end()) {
		const glm::ivec3 chunkPos = *it2;
		if (!chunks.contains(chunkPos)) {
			chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
			changedChunkPositions.insert(chunkPos);
		}

		++it2;
	}
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
		if (!chunks.contains(chunkPos)) {
			++it;
			continue;
		}
		
		Chunk& chunk = *chunks[chunkPos];
		if (chunk.hasMesh)
			chunk.mesh.free();

		std::vector<Vertex> chunkVerts{};
		std::vector<GLuint> chunkTris{};
		glm::mat4 chunkMatrix{};

		generateChunkMesh(chunkVerts, chunkTris, chunkMatrix,
			chunkSize, atlas, chunk, chunks);

		chunk.mesh = { chunkVerts, chunkTris, chunkMatrix };
		chunk.hasMesh = true;

		++it;
	}

	changedChunkPositions.clear();
}

void World::free() {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		delete it->second;
		++it;
	}

	chunks.clear();
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