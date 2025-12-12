#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int3.hpp>

struct ChunkVertex {
public:
	glm::ivec3 position{};
	glm::vec3 normal{};
	glm::vec2 texUv{};

	ChunkVertex() = default;
	ChunkVertex(const glm::ivec3& position)
		: position(position) {
	}

	ChunkVertex(const glm::ivec3& position, const glm::vec3& normal)
		: position(position), normal(normal) {
	}

	ChunkVertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texUv)
		: position(position), normal(normal), texUv(texUv) {
	}

};