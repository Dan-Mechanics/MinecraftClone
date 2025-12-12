#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int3.hpp>

struct IntVertex {
public:
	glm::ivec3 position{};
	glm::vec3 normal{};
	glm::vec2 texUv{};

	IntVertex() = default;
	IntVertex(const glm::ivec3& position)
		: position(position) {
	}

	IntVertex(const glm::ivec3& position, const glm::vec3& normal)
		: position(position), normal(normal) {
	}

	IntVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texUv)
		: position(position), normal(normal), texUv(texUv) {
	}

};