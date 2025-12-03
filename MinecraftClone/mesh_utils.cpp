#include "mesh_utils.h"

const BlockPos UP = { 0, 1, 0 };
const BlockPos DOWN = { 0, -1, 0 };
const BlockPos LEFT = { -1, 0, 0 };
const BlockPos RIGHT = { 1, 0, 0 };
const BlockPos FORWARD = { 0, 0, 1 };
const BlockPos BACK = { 0, 0, -1 };

void getChunk(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
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

			setCurrentFaceUvs(verts, atlas[blockType][Direction::UP]);

			faceCount++;
		}

		// DOWN. ===
		if (!has(blockPos + DOWN, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::DOWN]);

			faceCount++;
		}

		// FORWARD. ===
		if (!has(blockPos + FORWARD, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::FORWARD]);

			faceCount++;
		}

		// RIGHT. ===
		if (!has(blockPos + RIGHT, world)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::RIGHT]);

			faceCount++;
		}

		// BACK. ===
		if (!has(blockPos + BACK, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::BACK]);

			faceCount++;
		}

		// LEFT. ===
		if (!has(blockPos + LEFT, world)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::LEFT]);

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

	return world[chunkPos].contains(blockPos);
}

std::vector<glm::vec2> tilePositionToUvs(const int x, const int y) {
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
	atlas.insert({ BlockType::NYCELIUM, generateFillUvs() });

	atlas[BlockType::NYCELIUM][Direction::FORWARD] = tilePositionToUvs(0, 0);
	setEquatorUvs(atlas[BlockType::NYCELIUM][Direction::FORWARD], atlas[BlockType::NYCELIUM]);
	atlas[BlockType::NYCELIUM][Direction::UP] = tilePositionToUvs(0, 3);
	atlas[BlockType::NYCELIUM][Direction::DOWN] = tilePositionToUvs(0, 1);

	// DYCELIUM ==
	atlas.insert({ BlockType::DYCELIUM, generateFillUvs() });

	atlas[BlockType::DYCELIUM][Direction::FORWARD] = tilePositionToUvs(1, 0);
	setEquatorUvs(atlas[BlockType::NYCELIUM][Direction::FORWARD], atlas[BlockType::NYCELIUM]);
	atlas[BlockType::DYCELIUM][Direction::UP] = tilePositionToUvs(1, 3);
	atlas[BlockType::DYCELIUM][Direction::DOWN] = tilePositionToUvs(1, 1);

	// DIRT ==
	atlas.insert({ BlockType::DIRT, generateFillUvs() });

	atlas[BlockType::DIRT][Direction::UP] = tilePositionToUvs(0, 1);
	setAllUvs(atlas[BlockType::DIRT][Direction::UP], atlas[BlockType::DIRT]);

	// GRAVEL ==
	atlas.insert({ BlockType::GRAVEL, generateFillUvs() });

	atlas[BlockType::GRAVEL][Direction::UP] = tilePositionToUvs(1, 1);
	setAllUvs(atlas[BlockType::GRAVEL][Direction::UP], atlas[BlockType::GRAVEL]);

	// SAPPHIRE ==
	atlas.insert({ BlockType::SAPPHIRE, generateFillUvs() });

	atlas[BlockType::SAPPHIRE][Direction::UP] = tilePositionToUvs(0, 1);
	setAllUvs(atlas[BlockType::SAPPHIRE][Direction::UP], atlas[BlockType::SAPPHIRE]);

	// DIAMOND ==
	atlas.insert({ BlockType::DIAMOND, generateFillUvs() });

	atlas[BlockType::DIAMOND][Direction::UP] = tilePositionToUvs(1, 1);
	setAllUvs(atlas[BlockType::DIAMOND][Direction::UP], atlas[BlockType::DIAMOND]);

	return atlas;
}

WORLD generateWorld() {
	// EXAMPLE !!

	CHUNK chunk1 {
		{ { 0, 0, 0 }, BlockType::DIRT },
		{ { 0, 1, 0 }, BlockType::NYCELIUM }
	};

	WORLD world {
		{ { 0, 0, 0 }, chunk1 }
	};

	return world;
}

void setAllUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs) {
	allUvs[Direction::UP] = uvs;
	allUvs[Direction::DOWN] = uvs;
	setEquatorUvs(uvs, allUvs);
}

void setEquatorUvs(const std::vector<glm::vec2>& uvs, std::vector<std::vector<glm::vec2>>& allUvs) {
	allUvs[Direction::FORWARD] = uvs;
	allUvs[Direction::BACK] = uvs;
	allUvs[Direction::LEFT] = uvs;
	allUvs[Direction::RIGHT] = uvs;
}

std::vector<std::vector<glm::vec2>> generateFillUvs() {
	std::vector<std::vector<glm::vec2>> uvs{};
	for (int i = 0; i < 6; ++i) {
		uvs.emplace_back();
	}

	return uvs;
}
