#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int renderDistance)
	: chunkSize{ chunkSize }, renderDistance{ renderDistance } { 

	maxRenderDistance = renderDistance + 1;
	minRenderDistance = renderDistance - 1;

	noise.SetFractalOctaves(3);
	noise.SetFractalLacunarity(4.0f);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);

	/*reloadChunkInterval = 0.035f;
	updateChunksInterval = 0.2f;*/

	reloadChunkInterval = 0.04f;
	updateChunksInterval = 0.25f;
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

void World::update(const float dt, const Atlas& atlas, ThreadPool& pool, const glm::vec3& playerPos) {
	updateChunksTimer += dt;
	reloadChunkTimer += dt;

	if (reloadChunkTimer >= reloadChunkInterval) {
		reloadChunkTimer = 0.0f;
		reloadSingleChunkMesh(pool, atlas);
		return;
	}

	/*if (!changedChunkPositions.empty())
		return;*/

	/*removeOutsideRenderDistance(pool, playerPos);
	addInsideRenderDistance(pool, playerPos);*/
	if (updateChunksTimer >= updateChunksInterval) {
		updateChunksTimer = 0.0f;

		if (addOrRemoveToggle) {
			addInsideRenderDistance(pool, playerPos);
		}
		else {
			removeOutsideRenderDistance(pool, playerPos);
		}

		addOrRemoveToggle = !addOrRemoveToggle;
	}
}

void World::addInsideRenderDistance(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);
	const auto height = 25.0f;

	std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash> heightMaps{};
	for (int x = -maxRenderDistance; x < maxRenderDistance; ++x) {
		for (int z = -maxRenderDistance; z < maxRenderDistance; ++z) {
			for (int y = -minRenderDistance; y < minRenderDistance; ++y) {
				const auto chunkPos = glm::ivec3{ x, y, z } + playerChunkPos;
				if (chunks.contains(chunkPos))
					continue;

				const auto heightMapPos = glm::ivec3{ chunkPos.x, 0, chunkPos.z };
				if(!heightMaps.contains(heightMapPos))
					heightMaps[heightMapPos] = getHeightMap(noise, height, heightMapPos.x, heightMapPos.z, chunkSize);

				// I THINK FILLCHUNK IS CAUSING LAG.
			 	if (fillChunk(chunkPos, chunkSize, heightMaps[heightMapPos], chunks))
					changedChunkPositions.insert(chunkPos);
			}
		}
	}
}

void World::removeOutsideRenderDistance(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);

	auto it = chunks.begin();
	while (it != chunks.end()) {
		const auto chunkPos = it->first;
		if (chunkPos.x > playerChunkPos.x + maxRenderDistance ||
			chunkPos.x < playerChunkPos.x - maxRenderDistance ||
			chunkPos.z > playerChunkPos.z + maxRenderDistance ||
			chunkPos.z < playerChunkPos.z - maxRenderDistance ||
			chunkPos.y > playerChunkPos.y + maxRenderDistance ||
			chunkPos.y < playerChunkPos.y - maxRenderDistance) {
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

	if (!chunks.contains(chunkPos))
		chunks[chunkPos] = new Chunk{};

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
	auto it = changedChunkPositions.begin();
	if (it == changedChunkPositions.end())
		return;

	const glm::ivec3 chunkPos = *it;
	changedChunkPositions.erase(chunkPos);

	if (!chunks.contains(chunkPos))
		return;

	// THIS CHUNK IS EMPTY SO WE CAN REMOVE IT.
	if (chunks[chunkPos]->blocks.empty()) {
		delete chunks[chunkPos];
		chunks.erase(chunkPos);
		std::cout << "if (chunks[chunkPos]->blocks.empty()) {" << std::endl;
		return;
	}

	Chunk& chunk = *chunks[chunkPos];
	if (chunk.hasMesh)
		chunk.chunkMesh.free();

	std::vector<ChunkVertex> verts{};
	std::vector<GLuint> tris{};
	verts.reserve(2000);
	tris.reserve(3000);

	generateChunkMesh(verts, tris, chunkSize, atlas, chunkPos, chunks);

	chunk.chunkMesh = { verts, tris };
	chunk.hasMesh = true;
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
