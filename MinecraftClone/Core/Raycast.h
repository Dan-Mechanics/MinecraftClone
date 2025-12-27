#pragma once
#include "glm/vec3.hpp"
#include <glm/geometric.hpp>

struct Raycast {
public:
	glm::vec3 origin{};
	glm::vec3 direction{};
	float range{};

	Raycast();
	Raycast(const glm::vec3& origin, glm::vec3 direction, float range);

};

