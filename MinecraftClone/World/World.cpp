#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int maxRenderDistance)
	: chunkSize{ chunkSize }, maxRenderDistance{ maxRenderDistance } { 
	noise.SetFractalOctaves(3);
	noise.SetFractalLacunarity(7.5f);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
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

void World::addInsideRenderDistance(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);
	const auto height = 25.0f;
	
	for (int x = -maxRenderDistance; x < maxRenderDistance; ++x) {
		for (int z = -maxRenderDistance; z < maxRenderDistance; ++z) {
			std::vector<int> heightMap = getHeightMap(noise, height, x + playerChunkPos.x, z + playerChunkPos.z, chunkSize);

			for (int y = -maxRenderDistance; y < maxRenderDistance; ++y) {
				const auto chunkPos = glm::ivec3{ x, y, z } + playerChunkPos;
				if (chunks.contains(chunkPos))
					continue;

				if (fillChunk(chunkPos, chunkSize, heightMap, chunks))
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
			// WE NEED TO DELETE THIS CHUNK.
			// IT IS OUT OF BOUNDS.
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

	//std::cout << "reloadSingleChunkMesh" << std::endl;
	const glm::ivec3 chunkPos = *it;
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

	// TODO:
	// SPLIT UP THE WORK INTO SIX SIDES
	// AND GIVE TO THREADPOOL.
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
