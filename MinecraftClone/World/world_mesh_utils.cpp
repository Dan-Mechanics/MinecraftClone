#include "world_mesh_utils.h"

void generateChunkMesh(std::vector<ChunkVertex>& verts, std::vector<GLuint>& tris,
	const int chunkSize, const Atlas& atlas, const glm::ivec3& chunkPos,
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks) {
	const auto low = 0;
	const auto high = 1;

	const auto& blocks = chunks.at(chunkPos)->blocks;
	auto it = blocks.begin();
	while (it != blocks.end()) {
		if (it->second == BlockType::WATER) {
			++it;
			continue;
		}
		
		const int startingVertIndex = verts.size();
		auto faceCount = 0;

		const auto& blockPos = it->first;
		const auto& blockType = it->second;

		// UP. ===
		if (!solidAssumptiveHas(blockPos + up, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, up);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, up);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, up);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, up);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::UP]);
			faceCount++;
		}

		// DOWN. ===
		if (!solidAssumptiveHas(blockPos + down, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, down);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, down);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, down);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, down);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::DOWN]);
			faceCount++;
		}

		// FORWARD. ===
		if (!solidAssumptiveHas(blockPos + forward, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, forward);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::FORWARD]);
			faceCount++;
		}

		// RIGHT. ===
		if (!solidAssumptiveHas(blockPos + right, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, right);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::RIGHT]);
			faceCount++;
		}

		// BACK. ===
		if (!solidAssumptiveHas(blockPos + back, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, back);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::BACK]);
			faceCount++;
		}

		// LEFT. ===
		if (!solidAssumptiveHas(blockPos + left, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, left);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::LEFT]);
			faceCount++;
		}

		// GENERATE TRIANGLES. ===
		for (int i = 0; i < faceCount; ++i) {
			tris.push_back(startingVertIndex + i * 4);
			tris.push_back(startingVertIndex + i * 4 + 1);
			tris.push_back(startingVertIndex + i * 4 + 2);
			tris.push_back(startingVertIndex + i * 4);
			tris.push_back(startingVertIndex + i * 4 + 2);
			tris.push_back(startingVertIndex + i * 4 + 3);
		}

		++it;
	}
}

void generateTranslucentChunkMesh(std::vector<ChunkVertex>& verts, std::vector<GLuint>& tris,
	const int chunkSize, const Atlas& atlas, const glm::ivec3& chunkPos,
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks) {
	const auto low = 0;
	const auto high = 1;

	const auto& blocks = chunks.at(chunkPos)->blocks;
	auto it = blocks.begin();
	while (it != blocks.end()) {
		if (it->second != BlockType::WATER) {
			++it;
			continue;
		}

		const int startingVertIndex = verts.size();
		auto faceCount = 0;

		const auto& blockPos = it->first;
		const auto& blockType = it->second;

		// UP. ===
		if (!assumptiveHas(blockPos + up, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, up);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, up);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, up);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, up);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::UP]);
			faceCount++;
		}

		// DOWN. ===
		if (!assumptiveHas(blockPos + down, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, down);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, down);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, down);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, down);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::DOWN]);
			faceCount++;
		}

		// FORWARD. ===
		if (!assumptiveHas(blockPos + forward, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, forward);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, forward);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::FORWARD]);
			faceCount++;
		}

		// RIGHT. ===
		if (!assumptiveHas(blockPos + right, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, high }, right);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, high }, right);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::RIGHT]);
			faceCount++;
		}

		// BACK. ===
		if (!assumptiveHas(blockPos + back, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ high, high, low }, back);
			verts.emplace_back(blockPos + glm::ivec3{ high, low, low }, back);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::BACK]);
			faceCount++;
		}

		// LEFT. ===
		if (!assumptiveHas(blockPos + left, chunks, chunkSize)) {
			verts.emplace_back(blockPos + glm::ivec3{ low, low, high }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, high }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, high, low }, left);
			verts.emplace_back(blockPos + glm::ivec3{ low, low, low }, left);

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::LEFT]);
			faceCount++;
		}

		// GENERATE TRIANGLES. ===
		for (int i = 0; i < faceCount; ++i) {
			tris.push_back(startingVertIndex + i * 4);
			tris.push_back(startingVertIndex + i * 4 + 1);
			tris.push_back(startingVertIndex + i * 4 + 2);
			tris.push_back(startingVertIndex + i * 4);
			tris.push_back(startingVertIndex + i * 4 + 2);
			tris.push_back(startingVertIndex + i * 4 + 3);
		}

		++it;
	}
}

void setCubeFacesAsBlockType(std::vector<Vertex>& verts, const Atlas& atlas, const BlockType blockType) {
	for (int i = 0; i < verts.size(); i += 4) {
		const auto& face = atlas.maps.at(blockType).faces[static_cast<Direction>(i / 4)];
		for (int j = 0; j < 4; ++j) {
			verts[i + j].texUv = face.uvs[j];
		}
	}
}

Face tilePositionToUVs(const int x, const int y) {
	const auto scale = 16.0f;
	const auto low = 0.0f;
	const auto high = 1.0f;

	const std::vector<glm::vec2> uvs {
		{ high, high },
		{ low,  high },
		{ low, low },
		{ high, low }
	};

	Face face{ uvs };
	translateUVs(face, 0.0f, -1.0f);
	scaleUVs(face, scale);
	translateUVs(face, x / scale, -y / scale);

	return face;
}

void scaleUVs(Face& face, const float scale) {
	auto it = face.uvs.begin();
	while (it != face.uvs.end()) {
		*it /= scale;
		++it;
	}
}

void translateUVs(Face& face, const float x, const float y) {
	auto it = face.uvs.begin();
	while (it != face.uvs.end()) {
		it->x += y;
		it->y += x;
		++it;
	}
}

void rotateUVsClockwise(Face& face) {
	std::rotate(face.uvs.begin(), face.uvs.begin() + 1, face.uvs.end());
}

void setCurrentFaceUVs(std::vector<ChunkVertex>& verts, const Face& face) {
	const int beginFaceVert = verts.size() - 4;
	for (int i = 0; i < 4; ++i) {
		verts[beginFaceVert + i].texUv = face.uvs[i];
	}
}

Atlas generateAtlas() {
	Atlas atlas{};

	// NYCELIUM. ===
	atlas.maps[BlockType::NYCELIUM] = generateEmptyMap();
	setEquatorUVs(atlas.maps[BlockType::NYCELIUM], tilePositionToUVs(0, 0));
	atlas.maps[BlockType::NYCELIUM].faces[Direction::UP] = tilePositionToUVs(0, 3);
	atlas.maps[BlockType::NYCELIUM].faces[Direction::DOWN] = tilePositionToUVs(0, 1);

	// DYCELIUM. ==
	atlas.maps[BlockType::DYCELIUM] = generateEmptyMap();
	setEquatorUVs(atlas.maps[BlockType::DYCELIUM], tilePositionToUVs(1, 0));
	atlas.maps[BlockType::DYCELIUM].faces[Direction::UP] = tilePositionToUVs(1, 3);
	atlas.maps[BlockType::DYCELIUM].faces[Direction::DOWN] = tilePositionToUVs(1, 1);

	// LOG. ===
	atlas.maps[BlockType::LOG] = generateEmptyMap();
	setEquatorUVs(atlas.maps[BlockType::LOG], tilePositionToUVs(2, 0));
	atlas.maps[BlockType::LOG].faces[Direction::UP] = tilePositionToUVs(2, 1);
	atlas.maps[BlockType::LOG].faces[Direction::DOWN] = tilePositionToUVs(2, 1);
	atlas.maps[BlockType::LEAVES] = generateUniformMap(tilePositionToUVs(2, 2));

	// ASH LOG. ===
	atlas.maps[BlockType::ASH_LOG] = generateEmptyMap();
	setEquatorUVs(atlas.maps[BlockType::ASH_LOG], tilePositionToUVs(3, 0));
	atlas.maps[BlockType::ASH_LOG].faces[Direction::UP] = tilePositionToUVs(3, 1);
	atlas.maps[BlockType::ASH_LOG].faces[Direction::DOWN] = tilePositionToUVs(3, 1);
	atlas.maps[BlockType::GLOW_BERRIES] = generateUniformMap(tilePositionToUVs(3, 2));

	// ===
	atlas.maps[BlockType::DIRT] = generateUniformMap(tilePositionToUVs(0, 1));
	atlas.maps[BlockType::GRAVEL] = generateUniformMap(tilePositionToUVs(1, 1));
	atlas.maps[BlockType::SAPPHIRE] = generateUniformMap(tilePositionToUVs(0, 2));
	atlas.maps[BlockType::DIAMOND] = generateUniformMap(tilePositionToUVs(1, 2));
	atlas.maps[BlockType::REACTOR] = generateUniformMap(tilePositionToUVs(4, 0));
	atlas.maps[BlockType::STEEL] = generateUniformMap(tilePositionToUVs(4, 1));
	atlas.maps[BlockType::WATER] = generateUniformMap(tilePositionToUVs(4, 2));

	return atlas;
}

glm::ivec3 posToBlockPos(const glm::vec3& pos) {
	return glm::ivec3{ floor(pos.x), floor(pos.y), floor(pos.z) };
}

void setEquatorUVs(Map& map, const Face& face) {
	map.faces[Direction::FORWARD] = face;
	map.faces[Direction::BACK] = face;
	map.faces[Direction::LEFT] = face;
	map.faces[Direction::RIGHT] = face;
}

void setPoleUVs(Map& map, const Face& face) {
	map.faces[Direction::UP] = face;
	map.faces[Direction::DOWN] = face;
}

Map generateUniformMap(const Face& face) {
	Map map{};
	for (int i = 0; i < 6; ++i) {
		map.faces.emplace_back(face);
	}

	return map;
}

Map generateEmptyMap() {
	Map map{};
	for (int i = 0; i < 6; ++i) {
		map.faces.emplace_back();
	}

	return map;
}

glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const int chunkSize) {
	return { 
		floor((float)blockPos.x / chunkSize),
		floor((float)blockPos.y / chunkSize),
		floor((float)blockPos.z / chunkSize) 
	};
}

bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	const auto chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos))
		return false;

	return chunks.at(chunkPos)->blocks.contains(blockPos);
}

bool solidAssumptiveHas(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	const auto chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos))
		return true;

	const auto& blocks = chunks.at(chunkPos)->blocks;
	return blocks.contains(blockPos) && blocks.at(blockPos) != BlockType::WATER;
}

bool assumptiveHas(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	const auto chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos))
		return true;
	
	return chunks.at(chunkPos)->blocks.contains(blockPos);
}
