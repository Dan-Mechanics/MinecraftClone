#pragma once
#include "minecraft_clone.h"

struct Vertex {
public:
	glm::vec3 position{};
	glm::vec3 normal{};
	glm::vec3 color{};
	glm::vec2 texUv{};

	Vertex() = default;
	Vertex(const glm::vec3& position)
		: position(position) {
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal)
		: position(position), normal(normal) {
	}

	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec3& color, const glm::vec2& texUv)
		: position(position), normal(normal), color(color), texUv(texUv) {
	}

};