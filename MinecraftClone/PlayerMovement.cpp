#include "PlayerMovement.h"

PlayerMovement::PlayerMovement() = default;
PlayerMovement::PlayerMovement(const float standardSpeed) : standardSpeed{ standardSpeed } { }

void PlayerMovement::update(GLFWwindow* window, const glm::vec3& bodyRight, const glm::vec3& bodyForward,
	const float deltaTime, const bool hasFocus, 
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	if (!hasFocus)
		return;

	glm::vec3 movement{};
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movement += bodyForward;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movement -= bodyRight;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movement -= bodyForward;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movement += bodyRight;

	if (glm::length(movement) > 0.0f)
		movement = glm::normalize(movement);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movement += worldUp;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movement -= worldUp;

	auto currentSpeed = standardSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		currentSpeed *= 3.0f;

	movement *= currentSpeed * deltaTime;
	const auto blockPos = posToBlockPos(pos);

	const auto halfPlayerSize = 0.5f;
	if (movement.x != 0.0f) {
		const auto xDir = movement.x > 0.0f ? 1.0f : -1.0f;
		pos += glm::vec3{ movement.x, 0.0f, 0.0f };
		if (has(posToBlockPos(pos + glm::vec3{ xDir * halfPlayerSize, 0.0f, 0.0f }), chunks, chunkSize))
			pos.x = blockPos.x + halfPlayerSize;
	}
	
	if (movement.y != 0.0f) {
		const auto yDir = movement.y > 0.0f ? 1.0f : -1.0f;
		pos += glm::vec3{ 0.0f, movement.y, 0.0f };
		if (has(posToBlockPos(pos + glm::vec3{ 0.0f, yDir * halfPlayerSize, 0.0f }), chunks, chunkSize))
			pos.y = blockPos.y + halfPlayerSize;
	}

	if (movement.z != 0.0f) {
		const auto zDir = movement.z > 0.0f ? 1.0f : -1.0f;
		pos += glm::vec3{ 0.0f, 0.0f, movement.z };
		if (has(posToBlockPos(pos + glm::vec3{ 0.0f, 0.0f, zDir * halfPlayerSize }), chunks, chunkSize))
			pos.z = blockPos.z + halfPlayerSize;
	}
}