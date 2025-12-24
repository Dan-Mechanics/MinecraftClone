#pragma once
#include "world_mesh_utils.h"

class PlayerMovement {
public:
	PlayerMovement();
	void update(const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);

private:
	glm::vec3 pos{};
	glm::ivec3 prevBlockPos{};
};

