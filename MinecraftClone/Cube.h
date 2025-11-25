#pragma once
#include "Mesh.h"

class Cube {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	Mesh faceMesh{};

	Cube();
	Cube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void draw(const Shader& shader, const Camera& camera, const glm::vec3& lightPos, const glm::vec4& lightColor) const;
	void move(const glm::vec3& vel, const double dt);
	void free() const;
};