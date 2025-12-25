#pragma once
#include "world_mesh_utils.h"

class PlayerMovement {
public:
	glm::vec3 pos{};

	PlayerMovement();
	PlayerMovement(const float standardSpeed);

	void move(GLFWwindow* window, const glm::vec3& bodyRight,
		const glm::vec3& bodyForward, const float deltaTime, const bool hasFocus);

	void collideWithWorld(const float playerHalfSize, const float blockSize,
		const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);

private:
	glm::ivec3 prevBlockPos{};
	float standardSpeed{};
};

