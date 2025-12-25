#pragma once
#include "world_mesh_utils.h"

class PlayerMovement {
public:
	glm::vec3 pos{};

	PlayerMovement();
	PlayerMovement(const float standardSpeed, const glm::vec3& pos);

	void move(GLFWwindow* window, const glm::vec3& bodyRight,
		const glm::vec3& bodyForward, const float deltaTime, const bool hasFocus);

	void collideWithWorld(const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);

private:
	glm::vec3 vel{};
	bool previousSpacePressed{};
	float standardSpeed{};
	float eyeHeight{};

	void stopVerticalVelocity();
	glm::vec3 constrain(glm::vec3 pos, glm::ivec3& collisionOutput, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>&chunks, const int chunkSize) const;
	void jump();

};

