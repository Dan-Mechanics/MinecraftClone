#pragma once
#include "Mesh.h"

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

class Cube {
public:
	glm::vec3 position{};
	glm::vec3 rotation{};
	glm::vec3 scale{};
	Mesh mesh{};

	Cube();
	Cube(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);
	void draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) const;
	void move(const glm::vec3& vel, const double dt);
	void free() const;
};