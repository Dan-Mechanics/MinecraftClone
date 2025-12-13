#pragma once
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_int3.hpp>

struct ChunkVertex {
public:
	glm::ivec3 position{};
	glm::ivec3 normal{};
	glm::vec2 texUv{};

	ChunkVertex() = default;
	ChunkVertex(const glm::ivec3& position, const glm::ivec3& normal)
		: position{ position }, normal{ normal } { }

};