#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int maxRenderDistance)
	: chunkSize{ chunkSize }, maxRenderDistance{ maxRenderDistance } {
	maxDestroyDist = maxRenderDistance + 1;
	yMaxRendDist = maxRenderDistance - 1;
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

void World::allocateNewChunks(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);
	for (int x = -maxRenderDistance; x < maxRenderDistance; ++x) {
		for (int y = -yMaxRendDist; y < yMaxRendDist; ++y) {
			for (int z = -maxRenderDistance; z < maxRenderDistance; ++z) {
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
		if (chunkPos.x > playerChunkPos.x + maxDestroyDist ||
			chunkPos.x < playerChunkPos.x - maxDestroyDist ||
			chunkPos.z > playerChunkPos.z + maxDestroyDist ||
			chunkPos.z < playerChunkPos.z - maxDestroyDist ||
			chunkPos.y > playerChunkPos.y + maxDestroyDist ||
			chunkPos.y < playerChunkPos.y - maxDestroyDist) {
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
	notifyBlockChange(chunkPos, blockPos);
}

void World::remove(const glm::ivec3& blockPos) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos) || !chunks[chunkPos]->blocks.contains(blockPos))
		return;

	chunks[chunkPos]->blocks.erase(blockPos);
	notifyBlockChange(chunkPos, blockPos);
}

void World::reloadSingleChunkMesh(ThreadPool& pool, const Atlas& atlas) {
	if (changedChunkPositions.empty())
		return;

	const glm::ivec3 chunkPos = *changedChunkPositions.begin();
	changedChunkPositions.erase(chunkPos);

	if (!chunks.contains(chunkPos))
		return;

	// THIS CHUNK IS EMPTY SO WE CAN REMOVE IT.
	if (chunks[chunkPos]->blocks.empty()) {
		delete chunks[chunkPos];
		chunks.erase(chunkPos);
		return;
	}

	Chunk& chunk = *chunks[chunkPos];
	if (chunk.hasMesh)
		chunk.chunkMesh.free();

	std::vector<ChunkVertex> verts{};
	std::vector<GLuint> tris{};
	generateChunkMesh(verts, tris, chunkSize, atlas, chunkPos, chunks);
	chunk.chunkMesh = { verts, tris };
	chunk.hasMesh = true;
}

void World::flushAll(ThreadPool& pool, const Atlas& atlas) {
	std::vector<std::future<std::pair<std::vector<ChunkVertex>, std::vector<GLuint>>>> futures{};
	std::vector<glm::ivec3> positions{};

	auto it = changedChunkPositions.begin();
	while (it != changedChunkPositions.end()) {
		const glm::ivec3 chunkPos = *it;
		if (!chunks.contains(chunkPos)) {
			++it;
			continue;
		}

		// THIS CHUNK IS EMPTY SO WE CAN REMOVE IT.
		if (chunks[chunkPos]->blocks.empty()) {
			delete chunks[chunkPos];
			chunks.erase(chunkPos);

			++it;
			continue;
		}

		Chunk& chunk = *chunks[chunkPos];
		if (chunk.hasMesh)
			chunk.chunkMesh.free();

		// NOTE TO SELF:
		// YOU NEED TO MAKE SURE THE REFERENCES INPUT DOESN'T CHANGE.
		futures.emplace_back(pool.submit(getChunkMesh, chunkSize, std::ref(atlas), chunkPos, std::ref(chunks)));
		positions.emplace_back(chunkPos);

		++it;
	}

	changedChunkPositions.clear();

	for (size_t i = 0; i < futures.size(); ++i) {
		Chunk& chunk = *chunks[positions[i]];

		const auto pair = futures[i].get();
		chunk.chunkMesh = { pair.first, pair.second };
		chunk.hasMesh = true;
	}
}

int World::getChunkSize() const {
	return chunkSize;
}

std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& World::getChunks() {
	return chunks;
}

bool World::raycast(const Raycast& raycast, glm::ivec3& blockPos, glm::ivec3& normal) const {
	std::vector<AxisPlane> planes {
		AxisPlane{ { 1, 0, 0 }, raycast.origin, raycast.direction },
		AxisPlane{ { 0, 1, 0 }, raycast.origin, raycast.direction },
		AxisPlane{ { 0, 0, 1 }, raycast.origin, raycast.direction },
	};

	std::sort(planes.begin(), planes.end());
	glm::vec3 pointA = raycast.origin;
	glm::vec3 pointB = raycast.origin;

	while (planes[0].distance <= raycast.range) {
		if (validPositionsToBlockPos(planes[0].point, pointB, blockPos) && has(blockPos, chunks, chunkSize)) {
			if (validPositionsToBlockPos(pointA, pointB, normal))
				normal -= blockPos;

			return true;
		}

		pointA = pointB;
		pointB = planes[0].point;
		planes[0].advance();
		std::sort(planes.begin(), planes.end());
	}

	return false;
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

void World::notifyBlockChange(const glm::ivec3& chunkPos, glm::ivec3 blockPos) {
	changedChunkPositions.insert(chunkPos);
	blockPos -= chunkPos * chunkSize;

	if (blockPos.x <= 0)
		changedChunkPositions.insert(chunkPos + left);

	if (blockPos.x >= chunkSize - 1)
		changedChunkPositions.insert(chunkPos + right);

	if (blockPos.y <= 0)
		changedChunkPositions.insert(chunkPos + down);

	if (blockPos.y >= chunkSize - 1)
		changedChunkPositions.insert(chunkPos + up);

	if (blockPos.z <= 0)
		changedChunkPositions.insert(chunkPos + back);

	if (blockPos.z >= chunkSize - 1)
		changedChunkPositions.insert(chunkPos + forward);
}
