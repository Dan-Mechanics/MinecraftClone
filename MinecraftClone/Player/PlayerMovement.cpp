#include "PlayerMovement.h"

PlayerMovement::PlayerMovement() = default;
PlayerMovement::PlayerMovement(const float standardSpeed, const glm::vec3& pos)
	: standardSpeed{ standardSpeed }, pos{ pos } { }

void PlayerMovement::update(GLFWwindow* window, const glm::vec3& bodyRight, const glm::vec3& bodyForward, const float deltaTime) {
	if (!waterlogged) {
		move(window, bodyRight, bodyForward, deltaTime);
	}
	else {
		moveInWater(window, bodyRight, bodyForward, deltaTime);
	}
}

void PlayerMovement::move(GLFWwindow* window, const glm::vec3& bodyRight, const glm::vec3& bodyForward, const float deltaTime) {
	vel.y -= 10.0f * deltaTime;

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

	const auto spacePressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
	if (!previousSpacePressed && spacePressed)
		jump();

	previousSpacePressed = spacePressed;
	auto currentSpeed = standardSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		currentSpeed *= 2.0f;

	movement *= currentSpeed * deltaTime;
	movement += vel * deltaTime;
	pos += movement;
}

void PlayerMovement::moveInWater(GLFWwindow* window, const glm::vec3& bodyRight, const glm::vec3& bodyForward, const float deltaTime) {
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

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		vel.y = 5.0f;
	}
	else {
		vel.y = -4.0f;
	}

	movement *= standardSpeed * deltaTime;
	movement += vel * deltaTime;
	pos += movement;
}

void PlayerMovement::collideWithWorld(const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) {
	glm::ivec3 collisionOutput{};
	pos = constrain(pos - worldUp, collisionOutput, chunks, chunkSize) + worldUp;
	pos = constrain(pos, collisionOutput, chunks, chunkSize);

	if (collisionOutput.y > 0)
		stopVerticalVelocity();

	waterlogged = isBlock(posToBlockPos(pos), BlockType::WATER, chunkSize, chunks);
}

glm::vec3 PlayerMovement::constrain(glm::vec3 pos, glm::ivec3& collisionOutput,
	const std::unordered_map<glm::ivec3, Chunk*, ivec3hash>& chunks, const int chunkSize) const {
	const glm::vec3 halfExtents{ 0.25f, 0.5f, 0.25f };
	const auto blockSize = 1.0f;

	// CENTER. ===
	const auto centerBlockPos = posToBlockPos(pos);
	if (isSolid(posToBlockPos(pos), false, chunkSize, chunks)) {
		pos.y = centerBlockPos.y + halfExtents.y + blockSize;
		collisionOutput.y++;
	}

	// UP DOWN. ===
	const auto upBlockPos = posToBlockPos(pos + glm::vec3{ 0.0f, halfExtents.y, 0.0f });
	if (isSolid(upBlockPos, false, chunkSize, chunks)) {
		pos.y = upBlockPos.y - halfExtents.y;
		collisionOutput.y++;
	}

	const auto downBlockPos = posToBlockPos(pos - glm::vec3{ 0.0f, halfExtents.y, 0.0f });
	if (isSolid(downBlockPos, false, chunkSize, chunks)) {
		pos.y = downBlockPos.y + halfExtents.y + blockSize;
		collisionOutput.y++;
	}

	// FORWARD BACK. ===
	const auto forwardBlockPos = posToBlockPos(pos + glm::vec3{ 0.0f, 0.0f, halfExtents.z });
	if (isSolid(forwardBlockPos, false, chunkSize, chunks)) {
		pos.z = forwardBlockPos.z - halfExtents.z;
		collisionOutput.z++;
	}

	const auto backBlockPos = posToBlockPos(pos - glm::vec3{ 0.0f, 0.0f, halfExtents.z });
	if (isSolid(backBlockPos, false, chunkSize, chunks)) {
		pos.z = backBlockPos.z + halfExtents.z + blockSize;
		collisionOutput.z++;
	}

	// LEFT RIGHT. ===
	const auto rightBlockPos = posToBlockPos(pos + glm::vec3{ halfExtents.x, 0.0f, 0.0f });
	if (isSolid(rightBlockPos, false, chunkSize, chunks)) {
		pos.x = rightBlockPos.x - halfExtents.x;
		collisionOutput.x++;
	}

	const auto leftBlockPos = posToBlockPos(pos - glm::vec3{ halfExtents.x, 0.0f, 0.0f });
	if (isSolid(leftBlockPos, false, chunkSize, chunks)) {
		pos.x = leftBlockPos.x + halfExtents.x + blockSize;
		collisionOutput.x++;
	}

	return pos;
}

void PlayerMovement::stopVerticalVelocity() {
	vel.y = 0.0f;
}

void PlayerMovement::jump() {
	vel.y = 10.0f;
}
