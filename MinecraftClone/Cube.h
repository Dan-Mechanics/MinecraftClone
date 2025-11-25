#pragma once
#include "Mesh.h"

class Cube {
public:
	glm::vec3 pos{};
	glm::vec3 rot{};
	glm::vec3 scale{};
	glm::vec4 color{};
	Mesh mesh{};

	Cube();
	Cube(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
	void draw(const Shader& shader, const Camera& camera);
	void move(const glm::vec3& vel, const double dt);
	void setColor(const glm::vec4& color);
	void free() const;
};