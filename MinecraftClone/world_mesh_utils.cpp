#include "world_mesh_utils.h"

void generateChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const Atlas& atlas, const Chunk& chunk, const std::unordered_map<glm::ivec3, Chunk, hasing_utils::HashVec3>& chunks) {
	verts.clear();
	tris.clear();

	unsigned int chunkSize = 16;

	const auto low = 0.0f;
	const auto high = 1.0f;

	auto it = chunk.blocks.begin();
	while (it != chunk.blocks.end()) {
		const int startingVertIndex = verts.size();
		auto faceCount = 0;

		const glm::ivec3 blockPos = it->first;
		const BlockType blockType = it->second;

		glm::vec3 pos{ blockPos };
		pos.x = -pos.x - 1.0f;
		pos.z = -pos.z - 1.0f;

		// UP. ===
		if (!has(blockPos + up, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::UP]);
			faceCount++;
		}

		// DOWN. ===
		if (!has(blockPos + down, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::DOWN]);
			faceCount++;
		}

		// FORWARD. ===
		if (!has(blockPos + back, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::FORWARD]);
			faceCount++;
		}

		// RIGHT. ===
		if (!has(blockPos + left, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::RIGHT]);
			faceCount++;
		}

		// BACK. ===
		if (!has(blockPos + forward, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			setCurrentFaceUVs(verts, atlas.maps.at(blockType).faces[Direction::BACK]);
			faceCount++;
		}

		// LEFT. ===
		if (!has(blockPos + right, chunks, chunkSize)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

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

	modelMatrix = glm::mat4{ 1.0f };
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
	// rotateUVsClockwise(uvs);

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

void setCurrentFaceUVs(std::vector<Vertex>& verts, const Face& face) {
	auto beginFaceVert = verts.size() - 4;
	glm::vec3 magenta{ 1.0f, 0.0f, 1.0f };

	for (int i = 0; i < 4; ++i) {
		verts[beginFaceVert + i].texUv = face.uvs[i];
		verts[beginFaceVert + i].texUv = face.uvs[i];
		verts[beginFaceVert + i].texUv = face.uvs[i];
		verts[beginFaceVert + i].texUv = face.uvs[i];

		// MAKE THE VERTEX COLOR MAGENTA FOR DEBUG.
		// WE WON'T SEE THIS BECAUSE OF THE TEXTURE.
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
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

	// ===
	atlas.maps[BlockType::DIRT] = generateUniformMap(tilePositionToUVs(0, 1));
	atlas.maps[BlockType::GRAVEL] = generateUniformMap(tilePositionToUVs(1, 1));
	atlas.maps[BlockType::SAPPHIRE] = generateUniformMap(tilePositionToUVs(0, 2));
	atlas.maps[BlockType::DIAMOND] = generateUniformMap(tilePositionToUVs(1, 2));
	atlas.maps[BlockType::REACTOR] = generateUniformMap(tilePositionToUVs(2, 0));

	return atlas;
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

glm::ivec3 posToBlockPos(const glm::vec3& pos) {
	return glm::ivec3{ (int)pos.x, (int)pos.y, (int)pos.z };
}

glm::ivec3 blockPosToChunkPos(const glm::ivec3& blockPos, const unsigned int chunkSize) {
	return { blockPos.x / chunkSize, blockPos.y / chunkSize, blockPos.z / chunkSize };
}

bool isChunkValid(const glm::ivec3& chunkPos, const std::unordered_map<glm::ivec3, Chunk>& chunks) {
	return chunks.contains(chunkPos) &&
		chunks.at(chunkPos).blocks.begin() != chunks.at(chunkPos).blocks.end();
}

bool has(const glm::ivec3& blockPos, const std::unordered_map<glm::ivec3, Chunk, hasing_utils::HashVec3>& chunks, const unsigned int chunkSize) {
	const auto chunkPos = blockPosToChunkPos(blockPos, chunkSize);
	if (!chunks.contains(chunkPos))
		return false;

	return chunks.at(chunkPos).blocks.contains(blockPos);
}
