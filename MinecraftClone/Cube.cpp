#include "Cube.h"

Vertex cubeVerticies[] = { //     COORDINATES     //
	Vertex{glm::vec3(-0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f, -0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f,  0.5f,  0.5f)},
	Vertex{glm::vec3(-0.5f,  0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f, -0.5f)},
	Vertex{glm::vec3(0.5f,  0.5f,  0.5f)}
};

GLuint cubeIndices[] = {
	0, 1, 3, 3, 1, 2,
	1, 5, 2, 2, 5, 6,
	5, 4, 6, 6, 4, 7,
	4, 0, 7, 7, 0, 3,
	3, 2, 7, 7, 2, 6,
	4, 5, 0, 0, 5, 1
};

Cube::Cube() = default;
Cube::Cube(const glm::vec3& pos, const glm::vec3& rotation, const glm::vec3& scale) : pos{ pos }, rotation{ rotation }, scale{ scale } {
	std::vector <Vertex> verts(cubeVerticies, cubeVerticies + sizeof(cubeVerticies) / sizeof(Vertex));
	std::vector <GLuint> tris(cubeIndices, cubeIndices + sizeof(cubeIndices) / sizeof(GLuint));
	std::vector<Texture> tex{};
	mesh = { verts, tris, tex };
}

void Cube::draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) const {
	glm::mat4 matrix{ 1.0f };
	glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };

	mesh.draw(shader, camera, matrix, pos, rotation, scale, lightPos, lightColor);
}

void Cube::move(const glm::vec3& vel, const double dt) {
	pos += vel * (float)dt;
}

void Cube::free() const {
	mesh.free();
}