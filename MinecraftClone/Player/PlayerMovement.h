#pragma once
#include "../World/world_mesh_utils.h"

class PlayerMovement {
public:
	glm::vec3 pos{};
	bool waterlogged{};

	PlayerMovement();
	PlayerMovement(const float standardSpeed, const glm::vec3& pos);
	
	void update(GLFWwindow* window, const glm::vec3& bodyRight,
		const glm::vec3& bodyForward, const float deltaTime);

	void collideWithWorld(const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize);

private:
	glm::vec3 vel{};
	bool previousSpacePressed{};
	float standardSpeed{};
	float eyeHeight{};

	void move(GLFWwindow* window, const glm::vec3& bodyRight,
		const glm::vec3& bodyForward, const float deltaTime);

	void moveInWater(GLFWwindow* window, const glm::vec3& bodyRight,
		const glm::vec3& bodyForward, const float deltaTime);

	void stopVerticalVelocity();
	glm::vec3 constrain(glm::vec3 pos, glm::ivec3& collisionOutput, const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>&chunks, const int chunkSize) const;
	void jump();

};

