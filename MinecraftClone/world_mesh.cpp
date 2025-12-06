#include "world_mesh.h"

void generateChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const ATLAS& atlas, const CHUNK& chunk, const WORLD& world) {
	verts.clear();
	tris.clear();

	const BlockPos up = { 0, 1, 0 };
	const BlockPos down = { 0, -1, 0 };
	const BlockPos left = { -1, 0, 0 };
	const BlockPos right = { 1, 0, 0 };
	const BlockPos forward = { 0, 0, 1 };
	const BlockPos back = { 0, 0, -1 };

	// THIS CHANGES THE ORIGIN OF THE MESH,
	// YOU CAN ALSO ACHIEVE SIMILAR EFFECT WITH MODELMATRIX.
	const auto low = 0.0f;
	const auto high = 1.0f;

	auto current = chunk.begin();
	while (current != chunk.end()) {
		const int blockStartVertIndex = verts.size();
		auto faceCount = 0;

		const BlockPos blockPos = current->first;
		const BlockType blockType = current->second;
		glm::vec3 pos = blockPos.getVec3();
		pos.x = -pos.x - 1.0f;
		pos.z = -pos.z - 1.0f;

		// UP. ===
		if (!has(blockPos + up, world)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::UP]);
			faceCount++;
		}

		// DOWN. ===
		if (!has(blockPos + down, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::DOWN]);
			faceCount++;
		}

		// FORWARD. ===
		if (!has(blockPos + back, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::FORWARD]);
			faceCount++;
		}

		// RIGHT. ===
		if (!has(blockPos + left, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::RIGHT]);
			faceCount++;
		}

		// BACK. ===
		if (!has(blockPos + forward, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::BACK]);
			faceCount++;
		}

		// LEFT. ===
		if (!has(blockPos + right, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

			setCurrentFaceUVs(verts, atlas.at(blockType)[Direction::LEFT]);
			faceCount++;
		}

		// GENERATE TRIANGLES. ===
		for (int i = 0; i < faceCount; ++i) {
			tris.push_back(blockStartVertIndex + i * 4);
			tris.push_back(blockStartVertIndex + i * 4 + 1);
			tris.push_back(blockStartVertIndex + i * 4 + 2);
			tris.push_back(blockStartVertIndex + i * 4);
			tris.push_back(blockStartVertIndex + i * 4 + 2);
			tris.push_back(blockStartVertIndex + i * 4 + 3);
		}

		++current;
	}

	modelMatrix = glm::mat4{ 1.0f };
}

bool has(const BlockPos& blockPos, const WORLD& world) {
	BlockPos chunkPos = blockPosToChunkPos(blockPos);
	if (!world.contains(chunkPos))
		return false;

	return world.at(chunkPos).contains(blockPos);
}

std::vector<glm::vec2> tilePositionToUVs(const int x, const int y) {
	const auto scale = 16.0f;
	const auto low = 0.0f;
	const auto high = 1.0f;

	std::vector<glm::vec2> uvs {
		{ high, high },
		{ low,  high },
		{ low, low },
		{ high, low },
	};

	translateUVs(uvs, 0.0f, -1.0f);
	scaleUVs(uvs, scale);
	translateUVs(uvs, x / scale, -y / scale);

	// rotateUVsClockwise(uvs);

	return uvs;
}

void scaleUVs(std::vector<glm::vec2>& uvs, const float scale) {
	auto it = uvs.begin();
	while (it != uvs.end()) {
		*it /= scale;
		++it;
	}
}

void translateUVs(std::vector<glm::vec2>& uvs, const float x, const float y) {
	auto it = uvs.begin();
	while (it != uvs.end()) {
		it->x += y;
		it->y += x;
		++it;
	}
}

void rotateUVsClockwise(std::vector<glm::vec2>& uvs) {
	std::rotate(uvs.begin(), uvs.begin() + 1, uvs.end());
}

void setCurrentFaceUVs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs) {
	auto beginFaceVert = verts.size() - 4;
	glm::vec3 magenta{ 1.0f, 0.0f, 1.0f };

	for (int i = 0; i < 4; ++i) {
		verts[beginFaceVert + i].texUv = uvs[i];
		verts[beginFaceVert + i].texUv = uvs[i];
		verts[beginFaceVert + i].texUv = uvs[i];
		verts[beginFaceVert + i].texUv = uvs[i];

		// MAKE THE VERTEX COLOR MAGENTA FOR DEBUG.
		// WE WON'T SEE THIS BECAUSE OF THE TEXTURE.
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
		verts[beginFaceVert + i].color = magenta;
	}
}

BlockPos blockPosToChunkPos(const BlockPos& blockPos) {
	// ROUNDING DOWN.
	return { blockPos.x / 16, blockPos.y / 16, blockPos.z / 16 };
}

ATLAS generateAtlas() {
	ATLAS atlas{};

	// NYCELIUM. ===
	atlas[BlockType::NYCELIUM] = generateFillerMap();
	setEquatorUVs(atlas[BlockType::NYCELIUM], tilePositionToUVs(0, 0));
	atlas[BlockType::NYCELIUM][Direction::UP] = tilePositionToUVs(0, 3);
	atlas[BlockType::NYCELIUM][Direction::DOWN] = tilePositionToUVs(0, 1);

	// DYCELIUM. ==
	atlas[BlockType::DYCELIUM] = generateFillerMap();
	setEquatorUVs(atlas[BlockType::DYCELIUM], tilePositionToUVs(1, 0));
	atlas[BlockType::DYCELIUM][Direction::UP] = tilePositionToUVs(1, 3);
	atlas[BlockType::DYCELIUM][Direction::DOWN] = tilePositionToUVs(1, 1);

	// ===
	atlas[BlockType::DIRT] = generateUniformMap(tilePositionToUVs(0, 1));
	atlas[BlockType::GRAVEL] = generateUniformMap(tilePositionToUVs(1, 1));
	atlas[BlockType::SAPPHIRE] = generateUniformMap(tilePositionToUVs(0, 2));
	atlas[BlockType::DIAMOND] = generateUniformMap(tilePositionToUVs(1, 2));
	atlas[BlockType::REACTOR] = generateUniformMap(tilePositionToUVs(2, 0));

	return atlas;
}

WORLD generateDemoWorld() {
	CHUNK chunk{};
	for (int x = 0; x < 16; ++x) {
		for (int y = 0; y < 16; ++y) {
			for (int z = 0; z < 16; ++z) {
				if (randomInclusive(0, 1)) {
					chunk[{x, y, z}] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
					// chunk[{x + 50, y, z + 50}] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
					// chunk[{x + 50, y + 50, z + 50}] = static_cast<BlockType>(randomInclusive(0, BlockType::REACTOR));
				}
			}
		}
	}

	WORLD world{};
	world[{0, 0, 0}] = chunk;

	return world;
}

void setEquatorUVs(MAP& map, const std::vector<glm::vec2>& to) {
	map[Direction::FORWARD] = to;
	map[Direction::BACK] = to;
	map[Direction::LEFT] = to;
	map[Direction::RIGHT] = to;
}

void setPoleUVs(MAP& map, const std::vector<glm::vec2>& to) {
	map[Direction::UP] = to;
	map[Direction::DOWN] = to;
}

MAP generateUniformMap(const std::vector<glm::vec2>& to) {
	MAP map{};
	for (int i = 0; i < 6; ++i) {
		map.emplace_back(to);
	}

	return map;
}

MAP generateFillerMap() {
	MAP map{};
	for (int i = 0; i < 6; ++i) {
		map.emplace_back();
	}

	return map;
}
