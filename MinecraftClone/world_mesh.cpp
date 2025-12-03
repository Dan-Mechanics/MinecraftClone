#include "world_mesh.h"

const BlockPos UP = { 0, 1, 0 };
const BlockPos DOWN = { 0, -1, 0 };
const BlockPos LEFT = { -1, 0, 0 };
const BlockPos RIGHT = { 1, 0, 0 };
const BlockPos FORWARD = { 0, 0, 1 };
const BlockPos BACK = { 0, 0, -1 };

void getChunkMesh(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const ATLAS& atlas, const CHUNK& chunk, const WORLD& world) {
	verts.clear();
	tris.clear();

	const auto low = 0.0f;
	const auto high = 1.0f;

	auto current = chunk.begin();
	while (current != chunk.end()) {
		const int blockStartVertIndex = verts.size();
		auto faceCount = 0;

		const BlockPos blockPos = current->first;
		const BlockType blockType = current->second;
		const glm::vec3 pos = blockPos.getVec3();

		// UP. ===
		if (!has(blockPos + UP, world)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::UP]);
			faceCount++;
		}

		// DOWN. ===
		if (!has(blockPos + DOWN, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::DOWN]);
			faceCount++;
		}

		// FORWARD. ===
		if (!has(blockPos + FORWARD, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::FORWARD]);
			faceCount++;
		}

		// RIGHT. ===
		if (!has(blockPos + RIGHT, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::RIGHT]);
			faceCount++;
		}

		// BACK. ===
		if (!has(blockPos + BACK, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::BACK]);
			faceCount++;
		}

		// LEFT. ===
		if (!has(blockPos + LEFT, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas.at(blockType)[Direction::LEFT]);
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

std::vector<glm::vec2> tilePositionToUvs(const int x, const int y) {
	// BECAUSE 256 / 16 !!
	const auto TILE_COUNT_SIDE = 16.0f;
	const auto margin = 0.001f;
	std::vector<glm::vec2> uvs{
		{ x / TILE_COUNT_SIDE + margin, y / TILE_COUNT_SIDE + margin },
		{ x / TILE_COUNT_SIDE + margin, (y + 1) / TILE_COUNT_SIDE - margin },
		{ (x + 1) / TILE_COUNT_SIDE - margin, (y + 1) / TILE_COUNT_SIDE - margin },
		{ (x + 1) / TILE_COUNT_SIDE - margin, y / TILE_COUNT_SIDE + margin }
	};

	return uvs;
}

void setCurrentFaceUvs(std::vector<Vertex>& verts, const std::vector<glm::vec2>& uvs) {
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
	return { blockPos.x / 16.0f, blockPos.y / 16.0f, blockPos.z / 16.0f };
}

ATLAS generateAtlas() {
	ATLAS atlas{};

	// NYCELIUM ===
	atlas.insert({ BlockType::NYCELIUM, generateFillerMap() });
	setEquatorUvs(atlas[BlockType::NYCELIUM], tilePositionToUvs(0, 0));
	atlas[BlockType::NYCELIUM][Direction::UP] = tilePositionToUvs(0, 3);
	atlas[BlockType::NYCELIUM][Direction::DOWN] = tilePositionToUvs(0, 1);

	// DYCELIUM ==
	atlas.insert({ BlockType::DYCELIUM, generateFillerMap() });
	setEquatorUvs(atlas[BlockType::DYCELIUM], tilePositionToUvs(1, 0));
	atlas[BlockType::DYCELIUM][Direction::UP] = tilePositionToUvs(1, 3);
	atlas[BlockType::DYCELIUM][Direction::DOWN] = tilePositionToUvs(1, 1);

	// DIRT ==
	atlas.insert({ BlockType::DIRT, generateFillerMap() });
	setAllUvs(atlas[BlockType::DIRT], tilePositionToUvs(0, 1));

	// GRAVEL ==
	atlas.insert({ BlockType::GRAVEL, generateFillerMap() });
	setAllUvs(atlas[BlockType::GRAVEL], tilePositionToUvs(1, 1));

	// SAPPHIRE ==
	atlas.insert({ BlockType::SAPPHIRE, generateFillerMap() });
	setAllUvs(atlas[BlockType::SAPPHIRE], tilePositionToUvs(0, 1));

	// DIAMOND ==
	atlas.insert({ BlockType::DIAMOND, generateFillerMap() });
	setAllUvs(atlas[BlockType::DIAMOND], tilePositionToUvs(1, 1));

	return atlas;
}

WORLD generateWorld() {
	CHUNK chunk1 {
		{ { 0, 0, 0 }, BlockType::DIRT },
		{ { 0, 1, 0 }, BlockType::NYCELIUM }
	};

	WORLD world {
		{ { 0, 0, 0 }, chunk1 }
	};

	return world;
}

void setAllUvs(MAP& map, const std::vector<glm::vec2>& to) {
	setPoleUvs(map, to);
	setEquatorUvs(map, to);
}

void setEquatorUvs(MAP& map, const std::vector<glm::vec2>& to) {
	map[Direction::FORWARD] = to;
	map[Direction::BACK] = to;
	map[Direction::LEFT] = to;
	map[Direction::RIGHT] = to;
}

void setPoleUvs(MAP& map, const std::vector<glm::vec2>& to) {
	map[Direction::UP] = to;
	map[Direction::DOWN] = to;
}

MAP generateFillerMap() {
	MAP map{};
	for (int i = 0; i < 6; ++i) {
		map.emplace_back();
	}

	return map;
}
