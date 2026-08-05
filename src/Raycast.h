#pragma once
#include "minecraft_clone.h"

struct Raycast {
public:
	glm::vec3 origin{};
	glm::vec3 direction{};
	float range{};

	Raycast();
	Raycast(const glm::vec3& origin, glm::vec3 direction, float range);

};

