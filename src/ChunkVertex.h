#pragma once
#include "minecraft_clone.h"

struct ChunkVertex {
public:
	glm::ivec3 position{};
	glm::ivec3 normal{};
	glm::vec2 texUv{};

	ChunkVertex() = default;
	ChunkVertex(const glm::ivec3& position, const glm::ivec3& normal)
		: position{ position }, normal{ normal } { }

};