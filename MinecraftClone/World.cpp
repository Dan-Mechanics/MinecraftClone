#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int renderRadius)
	: chunkSize{ chunkSize }, renderRadius{ renderRadius } { 
	for (int x = -renderRadius; x < renderRadius; ++x) {
		for (int y = -2; y <= 2; ++y) {
			for (int z = -renderRadius; z < renderRadius; ++z) {
				visibleArea.insert(glm::ivec3{ x, y, z });
			}
		}
	}
}

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

void World::tick(ThreadPool& pool, const glm::vec3& playerPos) {	
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);

	// REMOVE OLD. ===
	auto it1 = chunks.begin();
	while (it1 != chunks.end()) {
		if (!visibleArea.contains(it1->first - playerChunkPos)) {
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
		const glm::ivec3 chunkPos = *it2 + playerChunkPos;
		if (!chunks.contains(chunkPos)) {
			chunks[chunkPos] = new Chunk{ chunkPos, chunkSize };
			auto future = pool.submit(allocateChunkData, std::ref(chunks), chunkSize, chunkPos);
			future.get();

			changedChunkPositions.insert(chunkPos);
		}

		++it2;
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

void World::flush(ThreadPool& pool, const Atlas& atlas) {
	while (!changedChunkPositions.empty()) {
		smallFlush(pool, atlas);
	}
}

void World::smallFlush(ThreadPool& pool, const Atlas& atlas) {
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

	/*generateChunkMesh(chunkVerts,
		chunkTris, chunkSize, atlas, chunk, chunks);*/

	auto future = pool.submit(generateChunkMesh, std::ref(chunkVerts),
		std::ref(chunkTris), chunkSize, std::ref(atlas), std::ref(chunk), std::ref(chunks));

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