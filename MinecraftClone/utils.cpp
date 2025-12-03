#include "utils.h"

const BlockPos UP = { 0, 1, 0 };
const BlockPos DOWN = { 0, -1, 0 };
const BlockPos LEFT = { -1, 0, 0 };
const BlockPos RIGHT = { 1, 0, 0 };
const BlockPos FORWARD = { 0, 0, 1 };
const BlockPos BACK = { 0, 0, -1 };

const std::unordered_map<BlockType, std::vector<glm::vec2>> map{
	{ BlockType::NYCELIUM, tilePositionToUvs(0, 0) },
	{ BlockType::DIRT, tilePositionToUvs(0, 0) },
	{ BlockType::SAPPHIRE, tilePositionToUvs(0, 0) },
	{ BlockType::DYCELIUM, tilePositionToUvs(0, 0) },
	{ BlockType::GRAVEL, tilePositionToUvs(0, 0) },
	{ BlockType::DIAMOND, tilePositionToUvs(0, 0) }
};

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix) {
	verts.clear();
	tris.clear();

	int faceCount = 0;
	int vertIndexOffset = 0;

	float low = -0.5f;
	float high = 0.5f;

	// UP !!
	verts.emplace_back(glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	faceCount++;

	// DOWN !!
	verts.emplace_back(glm::vec3{ low, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, low, low }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	verts.emplace_back(glm::vec3{ low, low, high }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	faceCount++;

	// FORWARD !!
	verts.emplace_back(glm::vec3{ high, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	verts.emplace_back(glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	verts.emplace_back(glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	verts.emplace_back(glm::vec3{ low, low, high }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	faceCount++;

	// RIGHT !!
	verts.emplace_back(glm::vec3{ high, low, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, high, low }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, high, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ high, low, high }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	faceCount++;

	// BACK !!
	verts.emplace_back(glm::vec3{ low, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	verts.emplace_back(glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	verts.emplace_back(glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	verts.emplace_back(glm::vec3{ high, low, low }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	faceCount++;

	// LEFT !!
	verts.emplace_back(glm::vec3{ low, low, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ low, high, high }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ low, high, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ low, low, low }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	faceCount++;



	for (int i = 0; i < faceCount; ++i) {
		tris.push_back(vertIndexOffset + i * 4);
		tris.push_back(vertIndexOffset + i * 4 + 1);
		tris.push_back(vertIndexOffset + i * 4 + 2);
		tris.push_back(vertIndexOffset + i * 4);
		tris.push_back(vertIndexOffset + i * 4 + 2);
		tris.push_back(vertIndexOffset + i * 4 + 3);

		// UV ===
		switch (i) {
		case 0: // UP.
			verts[i * 4 + 0].texUv = { 0.0f, 0.5f };
			verts[i * 4 + 1].texUv = { 0.5f, 0.5f };
			verts[i * 4 + 2].texUv = { 0.5f, 1.0f };
			verts[i * 4 + 3].texUv = { 0.0f, 1.0f };
			break;
		case 1: // DOWN.
			verts[i * 4 + 0].texUv = { 0.5f, 0.0f };
			verts[i * 4 + 1].texUv = { 1.0f, 0.0f };
			verts[i * 4 + 2].texUv = { 1.0f, 0.5f };
			verts[i * 4 + 3].texUv = { 0.5f, 0.5f };
			break;
		default: // AROUND.
			verts[i * 4 + 0].texUv = { 0.0f, 0.0f };
			verts[i * 4 + 1].texUv = { 0.5f, 0.0f };
			verts[i * 4 + 2].texUv = { 0.5f, 0.5f };
			verts[i * 4 + 3].texUv = { 0.0f, 0.5f };
			break;
		}

		// MAGENTA VERTEX COLOR, NOT TEXTURE, FOR DEBUG.
		verts[i * 4 + 0].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 1].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 2].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 3].color = { 1.0f, 0.0f, 1.0f };
	}

	modelMatrix = glm::mat4{ 1.0f };
}

void getChunk(const std::unordered_set<BlockPos>& blocks, std::vector<Vertex>& verts,
	std::vector<GLuint>& tris, glm::mat4& modelMatrix) {
	verts.clear();
	tris.clear();

	/*const auto low = -0.5f;
	const auto high = 0.5f;*/
	const auto low = 0.0f;
	const auto high = 1.0f;

	auto blockPos = blocks.begin();
	while (blockPos != blocks.end()) {
		const int blockStartVertIndex = verts.size();
		auto faceCount = 0;

		auto pos = blockPos->getVec3();
		BlockType blockType = BlockType::DYCELIUM;

		// UP !!
		if (!blocks.contains(*blockPos + UP)) {
			verts.emplace_back(pos + glm::vec3{ low, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ low, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, high }, glm::vec3{ 0.0f, 1.0f, 0.0f });
			verts.emplace_back(pos + glm::vec3{ high, high, low }, glm::vec3{ 0.0f, 1.0f, 0.0f });

			setCurrentFaceUvs(verts, atlas[blockType][Direction::UP]);

			// OR YOU COULD CHANGE THE VERT CONSTRUCTOR.
			//auto beginFaceVert = verts.size() - 4;
			//verts[beginFaceVert + 0].texUv = { 0.0f, 0.5f };
			//verts[beginFaceVert + 1].texUv = { 0.5f, 0.5f };
			//verts[beginFaceVert + 2].texUv = { 0.5f, 1.0f };
			//verts[beginFaceVert + 3].texUv = { 0.0f, 1.0f };

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

std::vector<glm::vec2> tilePositionToUvs(const int x, const int y) {
	const auto margin = 0.001f;
	std::vector<glm::vec2> uvs{
		{ x / 16.0f + margin, y / 16.0f + margin },
		{ x / 16.0f + margin, (y + 1) / 16.0f - margin },
		{ (x + 1) / 16.0f - margin, (y + 1) / 16.0f - margin },
		{ (x + 1) / 16.0f - margin, y / 16.0f + margin }
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

void writeAllAroundFaceUVs(std::vector<Vertex>& verts) {
	auto beginFaceVert = verts.size() - 4;
	verts[beginFaceVert + 0].texUv = { 0.0f, 0.0f };
	verts[beginFaceVert + 1].texUv = { 0.5f, 0.0f };
	verts[beginFaceVert + 2].texUv = { 0.5f, 0.5f };
	verts[beginFaceVert + 3].texUv = { 0.0f, 0.5f };
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

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris, glm::mat4& modelMatrix) {
	verts.emplace_back(glm::vec3{ -0.5f, 0.0f, 0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 5.0f });
	verts.emplace_back(glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 5.0f });
	verts.emplace_back(glm::vec3{ 0.5f, 0.0f,  0.5f }, glm::vec3{ 0.0f, -1.0f, 0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });

	verts.emplace_back(glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{ -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.0f, 0.8f,  0.0f }, glm::vec3{ -0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 2.5f, 5.0f });

	verts.emplace_back(glm::vec3{ -0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.5f, -0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ 0.0f, 0.5f, -0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.0f, 0.8f,  0.0f }, glm::vec3{ 0.0f, 0.5f, -0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 2.5f, 5.0f });

	verts.emplace_back(glm::vec3{ 0.5f, 0.0f, -0.5f }, glm::vec3{ 0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.5f, 0.0f,  0.5f }, glm::vec3{ 0.8f, 0.5f,  0.0f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.0f, 0.8f,  0.0f }, glm::vec3{ 0.8f, 0.5f,  0.0f }, glm::vec3{ 0.92f, 0.86f, 0.76f }, glm::vec2{ 2.5f, 5.0f });

	verts.emplace_back(glm::vec3{ 0.5f, 0.0f,  0.5f }, glm::vec3{ 0.0f, 0.5f,  0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 5.0f, 0.0f });
	verts.emplace_back(glm::vec3{ -0.5f, 0.0f,  0.5f }, glm::vec3{ 0.0f, 0.5f,  0.8f }, glm::vec3{ 0.83f, 0.70f, 0.44f }, glm::vec2{ 0.0f, 0.0f });
	verts.emplace_back(glm::vec3{ 0.0f, 0.8f,  0.0f }, glm::vec3{ 0.0f, 0.5f,  0.8f }, glm::vec3{ 0.92f, 0.86f, 0.76f }, glm::vec2{ 2.5f, 5.0f });

	GLuint pyramidIndices[] = {
		0, 1, 2, // Bottom side
		0, 2, 3, // Bottom side
		4, 6, 5, // Left side
		7, 9, 8, // Non-facing side
		10, 12, 11, // Right side
		13, 15, 14 // Facing side
	};

	for (size_t i = 0; i < sizeof(pyramidIndices) / sizeof(GLuint); ++i) {
		tris.push_back(pyramidIndices[i]);
	}

	modelMatrix = glm::mat4{ 1.0f };
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

int randomInclusive(const int min, const int max) {
	return rand() % (max - min + 1) + min;
}