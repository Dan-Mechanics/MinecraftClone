#include "utils.h"

void getCube(std::vector<Vertex>& verts, std::vector<GLuint>& tris) {
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

		// UV
		verts[i * 4 + 0].texUv = { 0.0f, 0.0f };
		verts[i * 4 + 1].texUv = { 0.0f, 1.0f };
		verts[i * 4 + 2].texUv = { 1.0f, 1.0f };
		verts[i * 4 + 3].texUv = { 1.0f, 0.0f };

		// VERTEX COLOR ( NOT TEXTURE !! )
		verts[i * 4 + 0].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 1].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 2].color = { 1.0f, 0.0f, 1.0f };
		verts[i * 4 + 3].color = { 1.0f, 0.0f, 1.0f };
	}
}

void getPyramid(std::vector<Vertex>& verts, std::vector<GLuint>& tris) {
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
}
