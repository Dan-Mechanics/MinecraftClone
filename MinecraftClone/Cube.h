#pragma once
#include "Mesh.h"

void getCube(std::vector<Vertex>& cubeVerts, std::vector<GLuint>& tris) {
	cubeVerts.clear();
	tris.clear();
	
	int faceCount = 0;
	int vertIndexOffset = 0;

	// UP !!
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f });
	faceCount++;

	// DOWN !!
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, -1.0f, 0.0f });
	faceCount++;

	// FORWARD !!
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ 0.0f, 0.0f, 1.0f });
	faceCount++;

	// RIGHT !!
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 1.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f });
	faceCount++;

	// BACK !!
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 1.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	cubeVerts.emplace_back(glm::vec3{ 1.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, -1.0f });
	faceCount++;

	// LEFT !!
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 1.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 1.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 1.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	cubeVerts.emplace_back(glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ -1.0f, 0.0f, 0.0f });
	faceCount++;

	for (int i = 0; i < faceCount; i++) {
		tris.push_back(vertIndexOffset + i * 4);
		tris.push_back(vertIndexOffset + i * 4 + 1);
		tris.push_back(vertIndexOffset + i * 4 + 2);
		tris.push_back(vertIndexOffset + i * 4);
		tris.push_back(vertIndexOffset + i * 4 + 2);
		tris.push_back(vertIndexOffset + i * 4 + 3);

		// UV
		cubeVerts[i * 4 + 0].texUv = { 0.0f, 0.0f };
		cubeVerts[i * 4 + 1].texUv = { 0.0f, 1.0f };
		cubeVerts[i * 4 + 2].texUv = { 1.0f, 1.0f };
		cubeVerts[i * 4 + 3].texUv = { 1.0f, 0.0f };

		// VERTEX COLOR ( NOT TEXTURE !! )
		cubeVerts[i * 4 + 0].color = { 1.0f, 0.0f, 1.0f };
		cubeVerts[i * 4 + 1].color = { 1.0f, 0.0f, 1.0f };
		cubeVerts[i * 4 + 2].color = { 1.0f, 0.0f, 1.0f };
		cubeVerts[i * 4 + 3].color = { 1.0f, 0.0f, 1.0f };
	}
}

class Cube {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};
	Mesh mesh{};

	Cube();
	Cube(std::vector<Vertex>& verts, std::vector<GLuint>& tris, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void draw(const Shader& shader, const Camera& camera, glm::vec4 lightColor, glm::vec3 lightPos, glm::vec4 worldColor);
	void move(const glm::vec3& vel, const double dt);
	void setColor(const glm::vec4& color);
	//void setTexture(); // IS THIS ALLOWED EVEN ??
	void free() const;
};