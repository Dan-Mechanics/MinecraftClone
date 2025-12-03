#include "mesh_utils.h"

const BlockPos UP = { 0, 1, 0 };
const BlockPos DOWN = { 0, -1, 0 };
const BlockPos LEFT = { -1, 0, 0 };
const BlockPos RIGHT = { 1, 0, 0 };
const BlockPos FORWARD = { 0, 0, 1 };
const BlockPos BACK = { 0, 0, -1 };

void getChunk(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix,
	const std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>>& atlas,
	const std::unordered_map<BlockPos, BlockType>& chunk,
	const std::unordered_map<BlockPos, std::unordered_map<BlockPos, BlockType>>& allChunks) {
	verts.clear();
	tris.clear();

	const auto low = 0.0f;
	const auto high = 1.0f;

	auto current = chunk.begin();
	while (current != chunk.end()) {
		const int blockStartVertIndex = verts.size();
		auto faceCount = 0;

		const BlockPos blockPos = current->first;
		const auto blockType = current->second;
		auto pos = blockPos.getVec3();

		// UP !!
		if (!has(blockPos, chunk, (*current->first + UP)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			auto beginFaceVert = verts.size() - 4;
			verts[beginFaceVert + 0].texUv = { 0.0f, 0.5f };
			verts[beginFaceVert + 1].texUv = { 0.5f, 0.5f };
			verts[beginFaceVert + 2].texUv = { 0.5f, 1.0f };
			verts[beginFaceVert + 3].texUv = { 0.0f, 1.0f };

			faceCount++;
		}

		// DOWN !!
		if (!blocks.contains(*current + DOWN)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			auto beginFaceVert = verts.size() - 4;
			verts[beginFaceVert + 0].texUv = { 0.5f, 0.0f };
			verts[beginFaceVert + 1].texUv = { 1.0f, 0.0f };
			verts[beginFaceVert + 2].texUv = { 1.0f, 0.5f };
			verts[beginFaceVert + 3].texUv = { 0.5f, 0.5f };

			faceCount++;
		}

		// FORWARD !!
		if (!blocks.contains(*current + FORWARD)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// RIGHT !!
		if (!blocks.contains(*current + RIGHT)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// BACK !!
		if (!blocks.contains(*current + BACK)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// LEFT !!
		if (!blocks.contains(*current + LEFT)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// GENERATE TRIANGLES.
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

bool has(const BlockPos& blockPos, const std::unordered_map<BlockPos, std::unordered_map<BlockPos, BlockType>>& allChunks) {
	BlockPos chunkPos = blockPosToChunkPos(blockPos);
	if (!allChunks.contains(chunkPos))
		return false;

	// !FIX
	return allChunks[chunkPos].contains(blockPos);
}

void getNyceliumChunk(const std::unordered_set<BlockPos>& blocks, std::vector<Vertex>& verts,
	std::vector<GLuint>& tris, glm::mat4& modelMatrix) {
	verts.clear();
	tris.clear();

	const auto low = 0.0f;
	const auto high = 1.0f;

	auto blockPos = blocks.begin();
	while (blockPos != blocks.end()) {
		const int blockStartVertIndex = verts.size();
		auto faceCount = 0;
		auto pos = blockPos->getVec3();

		// UP !!
		if (!blocks.contains(*blockPos + UP)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			auto beginFaceVert = verts.size() - 4;
			verts[beginFaceVert + 0].texUv = { 0.0f, 0.5f };
			verts[beginFaceVert + 1].texUv = { 0.5f, 0.5f };
			verts[beginFaceVert + 2].texUv = { 0.5f, 1.0f };
			verts[beginFaceVert + 3].texUv = { 0.0f, 1.0f };

			faceCount++;
		}

		// DOWN !!
		if (!blocks.contains(*blockPos + DOWN)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });

			auto beginFaceVert = verts.size() - 4;
			verts[beginFaceVert + 0].texUv = { 0.5f, 0.0f };
			verts[beginFaceVert + 1].texUv = { 1.0f, 0.0f };
			verts[beginFaceVert + 2].texUv = { 1.0f, 0.5f };
			verts[beginFaceVert + 3].texUv = { 0.5f, 0.5f };

			faceCount++;
		}

		// FORWARD !!
		if (!blocks.contains(*blockPos + FORWARD)) {
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// RIGHT !!
		if (!blocks.contains(*blockPos + RIGHT)) {
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// BACK !!
		if (!blocks.contains(*blockPos + BACK)) {
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
			verts.emplace_back(pos + glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// LEFT !!
		if (!blocks.contains(*blockPos + LEFT)) {
			verts.emplace_back(pos + glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });

			writeAllAroundFaceUVs(verts);

			faceCount++;
		}

		// GENERATE TRIANGLES.
		for (int i = 0; i < faceCount; ++i) {
			tris.push_back(blockStartVertIndex + i * 4);
			tris.push_back(blockStartVertIndex + i * 4 + 1);
			tris.push_back(blockStartVertIndex + i * 4 + 2);
			tris.push_back(blockStartVertIndex + i * 4);
			tris.push_back(blockStartVertIndex + i * 4 + 2);
			tris.push_back(blockStartVertIndex + i * 4 + 3);
		}

		++blockPos;
	}

	modelMatrix = glm::mat4{ 1.0f };
}

void writeAllAroundFaceUVs(std::vector<Vertex>& verts) {
	auto beginFaceVert = verts.size() - 4;
	verts[beginFaceVert + 0].texUv = { 0.0f, 0.0f };
	verts[beginFaceVert + 1].texUv = { 0.5f, 0.0f };
	verts[beginFaceVert + 2].texUv = { 0.5f, 0.5f };
	verts[beginFaceVert + 3].texUv = { 0.0f, 0.5f };
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

	// uvs = new Vector2[]
	// {
	// 	new Vector2(xPos / 16f + .001f, yPos / 16f + .001f),
	// 	new Vector2(xPos / 16f + .001f, (yPos + 1) / 16f - .001f),
	// 	new Vector2((xPos + 1) / 16f - .001f, (yPos + 1) / 16f - .001f),
	// 	new Vector2((xPos + 1) / 16f - .001f, yPos / 16f + .001f),
	// };

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

std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>> generateAtlas() {
	std::unordered_map<BlockType, std::vector<std::vector<glm::vec2>>> atlas{};

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
