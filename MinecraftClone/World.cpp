#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int viewingDistance, const int verticalViewingDistance)
	: chunkSize{ chunkSize }, viewingDistance{ viewingDistance }, verticalViewingDistance{ verticalViewingDistance } { }

void World::drawShadows(const Shader& shader, const Camera& camera) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		if (it->second->hasMesh)
			it->second->chunkMesh.drawShadows(shader, camera);

		++it;
	}
}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	auto it = chunks.begin();
	while (it != chunks.end()) {
		if (it->second->hasMesh)
			it->second->chunkMesh.draw(shader, camera, lightPos, lightColor, worldColor, material);

		++it;
	}
}

void World::allocateNewChunks(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);

	for (int x = -viewingDistance; x < viewingDistance; ++x) {
		for (int y = -verticalViewingDistance; y < verticalViewingDistance; ++y) {
			for (int z = -viewingDistance; z < viewingDistance; ++z) {
				const auto chunkPos = glm::ivec3{ x, y, z } + playerChunkPos;
				if (chunks.contains(chunkPos))
					continue;

				// WE NEED TO LOAD A CHUNK IN.
				auto future = pool.submit(fillChunkData, std::ref(chunks), chunkSize, chunkPos);
				if (future.get())
					changedChunkPositions.insert(chunkPos);
			}
		}
	}
}

void World::destroyOldChunks(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);

	auto it = chunks.begin();
	while (it != chunks.end()) {
		const auto chunkPos = it->first;
		if (chunkPos.x > playerChunkPos.x + viewingDistance ||
			chunkPos.x < playerChunkPos.x - viewingDistance ||
			chunkPos.z > playerChunkPos.z + viewingDistance ||
			chunkPos.z < playerChunkPos.z - viewingDistance ||
			chunkPos.y > playerChunkPos.y + verticalViewingDistance ||
			chunkPos.y < playerChunkPos.y - verticalViewingDistance) {
			// WE NEED TO DELETE THIS CHUNK.
			// IT IS OUT OF BOUNDS.
			changedChunkPositions.insert(it->first);
			delete it->second;
			it = chunks.erase(it);
		}
		else {
			++it;
		}
	}
}

void World::add(const glm::ivec3& blockPos, const BlockType& blockType) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);

	// ADD NEW CHUNK.
	if (!chunks.contains(chunkPos)) 
		chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };

	// YOU CAN'T PLACE A BLOCK HERE, SPACE ALREADY TAKEN.
	if (chunks[chunkPos]->blocks.contains(blockPos))
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

void World::reloadSingleChunkMesh(ThreadPool& pool, const Atlas& atlas) {
	auto it = changedChunkPositions.begin();
	if (it == changedChunkPositions.end())
		return;

	const glm::ivec3 chunkPos = *it;
	changedChunkPositions.erase(chunkPos);

	if (!chunks.contains(chunkPos))
		return;

	if (chunks[chunkPos]->blocks.empty()) {
		delete chunks[chunkPos];
		chunks.erase(chunkPos);
		return;
	}

	Chunk& chunk = *chunks[chunkPos];
	if (chunk.hasMesh)
		chunk.chunkMesh.free();

	std::vector<ChunkVertex> chunkVerts{};
	std::vector<GLuint> chunkTris{};

	auto future = pool.submit(generateChunkMesh, std::ref(chunkVerts),
		std::ref(chunkTris), chunkSize, std::ref(atlas), std::ref(chunk.blocks), std::ref(chunks));

	future.get();

	chunk.chunkMesh = { chunkVerts, chunkTris };
	chunk.hasMesh = true;
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