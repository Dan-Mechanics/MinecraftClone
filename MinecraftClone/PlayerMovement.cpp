#include "PlayerMovement.h"

PlayerMovement::PlayerMovement() = default;
PlayerMovement::PlayerMovement(const float standardSpeed) : standardSpeed{ standardSpeed } { }

void PlayerMovement::move(GLFWwindow* window, const glm::vec3& bodyRight, const glm::vec3& bodyForward,
	const float deltaTime, const bool hasFocus) {
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
	pos += movement;
}

void PlayerMovement::collideWithWorld(const float playerHalfSize, const float blockSize,
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	const auto upBlockPos = posToBlockPos(pos + glm::vec3{ 0.0f, playerHalfSize, 0.0f });
	// UP DOWN. ===
	if (has(upBlockPos, chunks, chunkSize))
		pos.y = upBlockPos.y - playerHalfSize;

	const auto downBlockPos = posToBlockPos(pos - glm::vec3{ 0.0f, playerHalfSize, 0.0f });
	if (has(downBlockPos, chunks, chunkSize))
		pos.y = downBlockPos.y + playerHalfSize + blockSize;

	// FORWARD BACK. ===
	const auto forwardBlockPos = posToBlockPos(pos + glm::vec3{ 0.0f, 0.0f, playerHalfSize });
	if (has(forwardBlockPos, chunks, chunkSize))
		pos.z = forwardBlockPos.z - playerHalfSize;

	const auto backBlockPos = posToBlockPos(pos - glm::vec3{ 0.0f, 0.0f, playerHalfSize });
	if (has(backBlockPos, chunks, chunkSize))
		pos.z = backBlockPos.z + playerHalfSize + blockSize;

	// RIGHT LEFT. ===
	const auto rightBlockPos = posToBlockPos(pos + glm::vec3{ playerHalfSize, 0.0f, 0.0f });
	if (has(rightBlockPos, chunks, chunkSize))
		pos.x = rightBlockPos.x - playerHalfSize;

	const auto leftBlockPos = posToBlockPos(pos - glm::vec3{ playerHalfSize, 0.0f, 0.0f });
	if (has(leftBlockPos, chunks, chunkSize))
		pos.x = leftBlockPos.x + playerHalfSize + blockSize;
}
