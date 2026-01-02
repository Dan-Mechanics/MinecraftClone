#include "World.h"

World::World() = default;
World::World(const int chunkSize, const int renderDistance)
	: chunkSize{ chunkSize }, renderDistance{ renderDistance } { 
	largeRenderDistance = renderDistance + 1;
	smallRenderDistance = renderDistance - 1;

	noise.SetFractalOctaves(3);
	noise.SetFractalLacunarity(4.0f);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	noise.SetFractalType(FastNoiseLite::FractalType_FBm);
	height = 25.0f;

	reloadChunkInterval = 0.025f;
	updateChunksInterval = 0.125f;

	blueTree = makeTreeStamp(14, 4, BlockType::LOG, BlockType::LEAVES);
	ashTree = makeTreeStamp(14, 4, BlockType::ASH_LOG, BlockType::GLOW_BERRIES);
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

	if (updateChunksTimer >= updateChunksInterval) {
		updateChunksTimer = 0.0f;
		if (toggle) {
			addNewChunksAsync(pool, playerPos);
		}
		else {
			removeOldChunksAsync(pool, playerPos);
		}

		toggle = !toggle;
	}
}

void World::addNewChunksAsync(ThreadPool& pool, const glm::vec3& playerPos) {
	std::vector<std::future<std::unordered_map<glm::ivec3, BlockType, ivec3hash>>> futures{};
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);
	std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash> heightMaps{};
	std::vector<int> relevantChunkIndices{};

	for (int x = -renderDistance; x < renderDistance; ++x) {
		for (int z = -renderDistance; z < renderDistance; ++z) {
			for (int y = -smallRenderDistance; y < smallRenderDistance; ++y) {
				const glm::ivec3 chunkPos = glm::ivec3{ x + playerChunkPos.x, y + playerChunkPos.y, z + playerChunkPos.z };
				if (chunks.contains(chunkPos))
					continue;

				const auto heightMapPos = flatten(chunkPos);
				if (!heightMaps.contains(heightMapPos))
					heightMaps[heightMapPos] = getHeightMap(noise, height, heightMapPos.x, heightMapPos.z, chunkSize);

				futures.emplace_back(pool.submit(fillChunkAsync, chunkPos, chunkSize, std::ref(heightMaps[heightMapPos])));
				if (chunkPosCache.size() < futures.size()) {
					chunkPosCache.emplace_back(chunkPos);
				}
				else {
					chunkPosCache[futures.size() - 1] = chunkPos;
				}
			}
		}
	}

	for (int i = 0; i < futures.size(); ++i) {
		auto blocks = futures[i].get();
		if (blocks.empty())
			continue;

		const auto& chunkPos = chunkPosCache[i];
		chunks[chunkPos] = new Chunk{};
		chunks[chunkPos]->blocks = std::move(blocks);

		relevantChunkIndices.push_back(i);
	}

	for (int i = 0; i < relevantChunkIndices.size(); ++i) {
		const auto& chunkPos = chunkPosCache[relevantChunkIndices[i]];
		applyStamp(blueTree, blueTree.getStandardOrigin(chunkPos, chunkSize, heightMaps));
		applyStamp(ashTree, ashTree.getStandardOrigin(chunkPos, chunkSize, heightMaps));

		changedChunkPositions.insert(chunkPos);
	}
}

void World::removeOldChunksAsync(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), chunkSize);
	std::vector<std::future<std::optional<glm::ivec3>>> futures{};
	futures.reserve(chunks.size());

	auto it = chunks.begin();
	while (it != chunks.end()) {
		futures.emplace_back(pool.submit(checkKeepChunkLoaded, it->first, std::ref(playerChunkPos), largeRenderDistance));
		++it;
	}

	for (int i = 0; i < futures.size(); ++i) {
		const auto& opt = futures[i].get();
		if (!opt.has_value())
			continue;
		
		delete chunks[opt.value()];
		chunks.erase(opt.value());
	}
}

void World::add(const glm::ivec3& blockPos, const BlockType& blockType) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, chunkSize);

	// THIS IS A RARE, BUT MUST BE ACCOUNTED FOR.
	if (!chunks.contains(chunkPos)) {
		chunks[chunkPos] = new Chunk{};
		chunks[chunkPos]->blocks = 
			fillChunkAsync(chunkPos, chunkSize, getHeightMap(noise, height, chunkPos.x, chunkPos.z, chunkSize));
	}

	notifyBlockChange(chunkPos, blockPos);

	// YOU CAN'T PLACE A BLOCK HERE, SPACE ALREADY TAKEN.
	if (chunks[chunkPos]->blocks.contains(blockPos))
		return;
	
	chunks[chunkPos]->blocks[blockPos] = blockType;
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

void World::applyStamp(const Stamp& stamp, const glm::ivec3& origin) {
	if (stamp.blocks.empty())
		return;

	if (randomInclusive(0, stamp.probability) != 0)
		return;

	auto it = stamp.blocks.begin();
	while (it != stamp.blocks.end()) {
		add(it->first + origin, it->second);
		++it;
	}
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
