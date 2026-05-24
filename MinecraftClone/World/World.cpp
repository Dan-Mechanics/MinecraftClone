#include "World.h"

World::World() = default;
World::World(const WorldSettings& worldSettings) : settings{ worldSettings } {
	reloadChunkMeshTimer = { 0.0125f };
	updateRendDistTimer = { 0.125f };
}

void World::drawShadows(const Shader& shader, const Camera& camera) {
	shader.activate();
	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");
	
	auto it = chunks.begin();
	while (it != chunks.end()) {
		if (it->second->mesh.hasData)
			it->second->mesh.draw();

		++it;
	}
}

void World::drawTranslucent(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {
	glm::mat4 modelMatrix = glm::identity<glm::mat4>();
	modelMatrix = glm::translate(modelMatrix, { 0.0f, -0.2f, 0.0f });

	shader.activate();
	// vao.bind();

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	glUniformMatrix4fv(glGetUniformLocation(shader.id, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniform3f(glGetUniformLocation(shader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(glGetUniformLocation(shader.id, "worldColor"), worldColor.x, worldColor.y, worldColor.z, worldColor.w);

	auto it = chunks.begin();
	while (it != chunks.end()) {
		if (it->second->translucentMesh.hasData)
			it->second->translucentMesh.draw();

		++it;
	}
}

void World::draw(const std::vector<Texture>& material, const Shader& shader, const Camera& camera,
	const glm::vec4& lightColor, const glm::vec3& lightPos, const glm::vec4& worldColor) {

	shader.activate();
	//vao.bind();

	glUniform3f(glGetUniformLocation(shader.id, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.sendMatrixToShader(shader, "camMatrix");

	glUniform3f(glGetUniformLocation(shader.id, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(glGetUniformLocation(shader.id, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform4f(glGetUniformLocation(shader.id, "worldColor"), worldColor.x, worldColor.y, worldColor.z, worldColor.w);


	auto it = chunks.begin();
	while (it != chunks.end()) {
		if (it->second->mesh.hasData)
			it->second->mesh.draw();

		++it;
	}
}

void World::update(const float deltaTime, const Atlas& atlas, ThreadPool& pool, const glm::vec3& playerPos) {
	if (reloadChunkMeshTimer.tick(deltaTime)) {
		reloadChunkMesh(pool, atlas);
		return;
	}

	if (updateRendDistTimer.tick(deltaTime)) {
		if (addNewChunksMode) {
			addNewChunks(pool, playerPos);
		}
		else {
			removeOldChunks(pool, playerPos);
		}

		addNewChunksMode = !addNewChunksMode;
	}
}

void World::addNewChunks(ThreadPool& pool, const glm::vec3& playerPos) {
	std::vector<std::future<std::unordered_map<glm::ivec3, BlockType, ivec3hash>>> futures{};
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), settings.chunkSize);
	std::unordered_map<glm::ivec3, std::vector<int>, ivec3hash> heightMaps{};
	std::vector<int> newChunkPosIndices{};

	for (int x = -settings.rendDist; x < settings.rendDist; ++x) {
		for (int z = -settings.rendDist; z < settings.rendDist; ++z) {
			for (int y = -settings.smallRendDist; y < settings.smallRendDist; ++y) {
				const glm::ivec3 chunkPos = glm::ivec3{ x + playerChunkPos.x, y + playerChunkPos.y, z + playerChunkPos.z };
				if (chunks.contains(chunkPos))
					continue;

				const auto heightMapPos = flatten(chunkPos);
				if (!heightMaps.contains(heightMapPos))
					heightMaps[heightMapPos] = getHeightMap(worldGen.noise, worldGen.height, heightMapPos.x, heightMapPos.z, settings.chunkSize);

				futures.emplace_back(pool.submit(fillChunk,
					chunkPos, settings.chunkSize, worldGen.waterHeight, std::ref(heightMaps[heightMapPos])));

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

		// ADD PENDING. ===
		const auto& chunkPos = chunkPosCache[i];
		if (pending.contains(chunkPos)) {
			const auto& pendingBlocks = pending[chunkPos];
			for (int i = 0; i < pendingBlocks.size(); ++i) {
				blocks[pendingBlocks[i].first] = pendingBlocks[i].second;
			}
		}

		if (blocks.empty())
			continue;

		chunks[chunkPos] = new Chunk{};
		chunks[chunkPos]->blocks = std::move(blocks);

		pending.erase(chunkPos);

		notifyChunkChange(chunkPos);
		newChunkPosIndices.push_back(i);
	}

	for (int i = 0; i < newChunkPosIndices.size(); ++i) {
		const auto& chunkPos = chunkPosCache[newChunkPosIndices[i]];
		applyStamp(worldGen.blueTree, getStandardStampOrigin(chunkPos, settings.chunkSize, heightMaps));
		applyStamp(worldGen.ashTree, getStandardStampOrigin(chunkPos, settings.chunkSize, heightMaps));
	}
}

void World::removeOldChunks(ThreadPool& pool, const glm::vec3& playerPos) {
	const auto playerChunkPos = blockPosToChunkPos(posToBlockPos(playerPos), settings.chunkSize);
	std::vector<std::future<std::optional<glm::ivec3>>> futures{};
	futures.reserve(chunks.size());

	auto it = chunks.begin();
	while (it != chunks.end()) {
		futures.emplace_back(pool.submit(checkKeepChunkLoaded, it->first, std::ref(playerChunkPos), settings.largeRendDist));
		++it;
	}

	for (int i = 0; i < futures.size(); ++i) {
		const auto& opt = futures[i].get();
		if (!opt.has_value())
			continue;
		
		delete chunks[opt.value()];
		chunks.erase(opt.value());
		pending.erase(opt.value());
	}
}

void World::add(const glm::ivec3& blockPos, const BlockType& blockType) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, settings.chunkSize);
	if (!chunks.contains(chunkPos)) {
		if (!pending.contains(chunkPos))
			pending[chunkPos] = {};

		pending[chunkPos].emplace_back(blockPos, blockType);
		return;
	}

	chunks[chunkPos]->blocks[blockPos] = blockType;
	notifyBlockChange(chunkPos, blockPos);

	if (blockType == BlockType::REACTOR)
		checkStructure(blockPos, worldGen.steelCore);
}

void World::remove(const glm::ivec3& blockPos) {
	const glm::ivec3 chunkPos = blockPosToChunkPos(blockPos, settings.chunkSize);
	if (!chunks.contains(chunkPos) || !chunks[chunkPos]->blocks.contains(blockPos))
		return;

	chunks[chunkPos]->blocks.erase(blockPos);
	notifyBlockChange(chunkPos, blockPos);
}

void World::reloadChunkMesh(ThreadPool& pool, const Atlas& atlas) {
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
	chunk.mesh.free();

	std::vector<ChunkVertex> verts{};
	std::vector<GLuint> tris{};
	verts.reserve(2000);
	tris.reserve(3000);

	generateChunkMesh(verts, tris, settings.chunkSize, atlas, chunkPos, chunks);
	chunk.mesh = { verts, tris };

	verts.clear();
	tris.clear();
	generateTranslucentChunkMesh(verts, tris, settings.chunkSize, atlas, chunkPos, chunks);
	chunk.translucentMesh = { verts, tris };
}

int World::getChunkSize() const {
	return settings.chunkSize;
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
		if (validPositionsToBlockPos(planes[0].point, pointB, blockPos) && isSolid(blockPos, false, settings.chunkSize, chunks)) {
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

	if (!stamp.spawnInWater && origin.y <= worldGen.waterHeight)
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

void World::checkStructure(const glm::ivec3& origin, const Stamp& stamp) {
	auto passed = true;
	auto it = stamp.blocks.begin();
	while (it != stamp.blocks.end()) {
		if (!isBlock(it->first + origin, it->second, settings.chunkSize, chunks)) {
			passed = false;
			break;
		}

		++it;
	}

	if (!passed)
		return;

	// REMOVE THE STAMP.
	it = stamp.blocks.begin();
	while (it != stamp.blocks.end()) {
		remove(it->first + origin);
		++it;
	}

	// REMOVE THE REACTOR BLOCK.
	remove(origin);
	pendingStructures.emplace(origin);
}

void World::notifyBlockChange(const glm::ivec3& chunkPos, glm::ivec3 blockPos) {
	changedChunkPositions.insert(chunkPos);
	blockPos -= chunkPos * settings.chunkSize;

	if (blockPos.x <= 0)
		changedChunkPositions.insert(chunkPos + left);

	if (blockPos.x >= settings.chunkSize - 1)
		changedChunkPositions.insert(chunkPos + right);

	if (blockPos.y <= 0)
		changedChunkPositions.insert(chunkPos + down);

	if (blockPos.y >= settings.chunkSize - 1)
		changedChunkPositions.insert(chunkPos + up);

	if (blockPos.z <= 0)
		changedChunkPositions.insert(chunkPos + back);

	if (blockPos.z >= settings.chunkSize - 1)
		changedChunkPositions.insert(chunkPos + forward);
}